#include "FileDownloader.h"

#include "CacheUtils.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QUrl>

namespace {

void attachSslHandler(QNetworkReply *reply)
{
    QObject::connect(reply, &QNetworkReply::sslErrors, reply, [reply](const QList<QSslError> &errors) {
        Q_UNUSED(errors);
        reply->ignoreSslErrors();
    });
}

} // namespace

FileDownloader::FileDownloader(QObject *parent)
    : QObject(parent)
    , m_netManager(new QNetworkAccessManager(this))
    , m_downloadReply(nullptr)
    , m_targetFile(nullptr)
    , m_totalFileSize(0)
    , m_downloadedBytes(0)
    , m_currentChunkStart(0)
    , m_currentChunkEnd(0)
    , m_isChunkDownload(false)
    , m_supportRangeRequest(false)
    , m_preserveDownloadedFile(false)
    , m_currentChunkIndex(0)
    , m_totalChunks(0)
{
}

FileDownloader::~FileDownloader()
{
    releaseDownloadResources();
}

void FileDownloader::startDownload(const QString &downloadUrl,
                                   const QString &userName,
                                   const QSslConfiguration &sslConf,
                                   const QString &accessToken)
{
    m_currentUser = userName;
    m_accessToken = accessToken;

    if (downloadUrl.trimmed().isEmpty()) {
        emit downloadFailed(QStringLiteral("下载地址为空"));
        return;
    }

    if (userName.trimmed().isEmpty()) {
        emit downloadFailed(QStringLiteral("当前用户为空"));
        return;
    }

    releaseDownloadResources();

    const QString downloadDir = createDownloadDir(userName);
    if (downloadDir.isEmpty()) {
        emit downloadFailed(QStringLiteral("无法创建下载目录"));
        return;
    }

    m_fileUrl = downloadUrl;
    m_sslConf = sslConf;
    m_totalFileSize = 0;
    m_downloadedBytes = 0;
    m_currentChunkStart = 0;
    m_currentChunkEnd = 0;
    m_isChunkDownload = false;
    m_supportRangeRequest = false;
    m_preserveDownloadedFile = false;
    m_currentChunkIndex = 0;
    m_totalChunks = 0;
    m_inFlightChunks.clear();
    m_replyToChunkMap.clear();

    QDir dir(downloadDir);
    m_savePath = dir.filePath(extractFileName(downloadUrl));

    m_targetFile = new QFile(m_savePath);
    if (!m_targetFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        const QString error = m_targetFile->errorString();
        releaseDownloadResources();
        emit downloadFailed(QStringLiteral("无法创建目标文件: %1").arg(error));
        return;
    }

    startChunkDownload(downloadUrl, userName, sslConf);
}

void FileDownloader::startChunkDownload(const QString &downloadUrl,
                                        const QString &userName,
                                        const QSslConfiguration &sslConf)
{
    Q_UNUSED(userName);

    QNetworkRequest request{QUrl(downloadUrl)};
    request.setSslConfiguration(sslConf);
    request.setRawHeader("User-Agent", "Mozilla/5.0");
    applyAuthorization(request);

    m_downloadReply = m_netManager->head(request);
    connect(m_downloadReply, &QNetworkReply::finished, this, &FileDownloader::onFileSizeReceived);
    attachSslHandler(m_downloadReply);
}

void FileDownloader::onFileSizeReceived()
{
    QNetworkReply *reply = m_downloadReply;
    if (reply == nullptr) {
        return;
    }

    if (restartDownloadForRedirect(reply)) {
        return;
    }

    const bool ok = (reply->error() == QNetworkReply::NoError);
    if (ok) {
        const QVariant contentLength = reply->header(QNetworkRequest::ContentLengthHeader);
        if (contentLength.isValid()) {
            m_totalFileSize = contentLength.toLongLong();
        }

        const QByteArray acceptRanges = reply->rawHeader("Accept-Ranges").trimmed().toLower();
        m_supportRangeRequest = (acceptRanges == "bytes");
    }

    reply->deleteLater();
    m_downloadReply = nullptr;

    if (!ok || m_totalFileSize <= 0 || !m_supportRangeRequest || m_totalFileSize <= CHUNK_DOWNLOAD_SIZE) {
        if (m_targetFile && m_targetFile->isOpen()) {
            m_targetFile->resize(0);
            m_targetFile->seek(0);
        }
        m_isChunkDownload = false;
        startDirectDownload();
        return;
    }

    if (m_targetFile && m_targetFile->isOpen()) {
        m_targetFile->resize(m_totalFileSize);
    }

    m_isChunkDownload = true;
    m_downloadedBytes = 0;
    m_currentChunkIndex = 0;
    m_totalChunks = static_cast<int>((m_totalFileSize + CHUNK_DOWNLOAD_SIZE - 1) / CHUNK_DOWNLOAD_SIZE);
    startConcurrentChunkDownload();
}

void FileDownloader::startDirectDownload()
{
    if (m_targetFile && m_targetFile->isOpen()) {
        m_targetFile->seek(0);
    }

    QNetworkRequest request{QUrl(m_fileUrl)};
    request.setSslConfiguration(m_sslConf);
    request.setRawHeader("User-Agent", "Mozilla/5.0");
    applyAuthorization(request);

    m_downloadReply = m_netManager->get(request);
    connect(m_downloadReply, &QNetworkReply::readyRead, this, &FileDownloader::onDownloadReadyRead);
    connect(m_downloadReply, &QNetworkReply::downloadProgress, this, &FileDownloader::onDownloadProgress);
    connect(m_downloadReply, &QNetworkReply::finished, this, &FileDownloader::onDownloadFinished);
    attachSslHandler(m_downloadReply);
}

void FileDownloader::downloadNextChunk()
{
    if (m_totalFileSize <= 0 || m_downloadedBytes >= m_totalFileSize) {
        if (m_targetFile && m_targetFile->isOpen()) {
            m_targetFile->flush();
            m_targetFile->close();
        }

        const QString finalPath = finalFilePathFromReply(nullptr).isEmpty()
                                      ? m_savePath
                                      : finalFilePathFromReply(nullptr);
        if (finalPath != m_savePath && QFile::exists(m_savePath)) {
            const QString desiredPath = uniqueSavePath(finalPath);
            QFile::rename(m_savePath, desiredPath);
            m_savePath = desiredPath;
        }

        m_preserveDownloadedFile = true;
        emit downloadFinished(m_savePath, true);
        releaseDownloadResources();
        return;
    }

    m_currentChunkStart = m_downloadedBytes;
    m_currentChunkEnd = qMin(m_currentChunkStart + CHUNK_DOWNLOAD_SIZE - 1, m_totalFileSize - 1);

    if (m_targetFile && m_targetFile->isOpen()) {
        m_targetFile->seek(m_currentChunkStart);
    }

    QNetworkRequest request{QUrl(m_fileUrl)};
    request.setSslConfiguration(m_sslConf);
    request.setRawHeader("User-Agent", "Mozilla/5.0");
    request.setRawHeader("Range",
                         QString("bytes=%1-%2").arg(m_currentChunkStart).arg(m_currentChunkEnd).toUtf8());
    applyAuthorization(request);

    m_downloadReply = m_netManager->get(request);
    connect(m_downloadReply, &QNetworkReply::readyRead, this, &FileDownloader::onDownloadReadyRead);
    connect(m_downloadReply, &QNetworkReply::finished, this, &FileDownloader::onChunkDownloadFinished);
    attachSslHandler(m_downloadReply);
}

void FileDownloader::onChunkDownloadFinished()
{
    QNetworkReply *reply = m_downloadReply;
    if (reply == nullptr) {
        return;
    }

    if (restartDownloadForRedirect(reply)) {
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        const QString error = reply->errorString();
        emit downloadFailed(error);
        releaseDownloadResources();
        return;
    }

    const QByteArray tail = reply->readAll();
    if (!tail.isEmpty() && m_targetFile && m_targetFile->isOpen()) {
        m_targetFile->write(tail);
        m_targetFile->flush();
    }

    m_downloadedBytes = m_currentChunkEnd + 1;
    emit downloadProgress(m_downloadedBytes, m_totalFileSize);

    reply->deleteLater();
    m_downloadReply = nullptr;

    downloadNextChunk();
}

QString FileDownloader::createDownloadDir(const QString &userName)
{
    const QString dirPath = CacheUtils::getFileDownloadRootDir(userName);
    if (dirPath.isEmpty()) {
        return QString();
    }

    QDir dir(dirPath);
    if (!dir.mkpath(".")) {
        return QString();
    }

    return dirPath;
}

QString FileDownloader::extractFileName(const QString &downloadUrl) const
{
    const QUrl url(downloadUrl);
    QString fileName = url.fileName();
    if (!fileName.isEmpty() && fileName != "download") {
        return fileName;
    }

    const QByteArray hash = QCryptographicHash::hash(downloadUrl.toUtf8(), QCryptographicHash::Md5);
    return QString("%1.bin").arg(QString::fromLatin1(hash.toHex()));
}

void FileDownloader::onDownloadReadyRead()
{
    if (m_downloadReply == nullptr || m_targetFile == nullptr || !m_targetFile->isOpen()) {
        return;
    }

    const QByteArray data = m_downloadReply->readAll();
    if (!data.isEmpty()) {
        m_targetFile->write(data);
        m_targetFile->flush();
    }
}

void FileDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_isChunkDownload) {
        return;
    }

    emit downloadProgress(bytesReceived, bytesTotal);
}

void FileDownloader::onDownloadFinished()
{
    QNetworkReply *reply = m_downloadReply;
    if (reply == nullptr) {
        return;
    }

    if (restartDownloadForRedirect(reply)) {
        return;
    }

    const bool ok = (reply->error() == QNetworkReply::NoError);
    if (m_targetFile && m_targetFile->isOpen()) {
        const QByteArray tail = reply->readAll();
        if (!tail.isEmpty()) {
            m_targetFile->write(tail);
            m_targetFile->flush();
        }
        m_targetFile->close();
    }

    if (!ok) {
        const QString error = reply->errorString();
        emit downloadFailed(error);
        releaseDownloadResources();
        return;
    }

    QString finalPath = finalFilePathFromReply(reply);
    if (finalPath.isEmpty()) {
        finalPath = m_savePath;
    }

    if (!finalPath.isEmpty() && finalPath != m_savePath && QFile::exists(m_savePath)) {
        const QString desiredPath = uniqueSavePath(finalPath);
        QFile currentFile(m_savePath);
        if (currentFile.rename(desiredPath)) {
            m_savePath = desiredPath;
        } else if (currentFile.copy(desiredPath)) {
            currentFile.remove();
            m_savePath = desiredPath;
        }
    }

    if (!m_savePath.isEmpty() && QFile::exists(m_savePath)) {
        const QFileInfo fileInfo(m_savePath);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
    }

    m_preserveDownloadedFile = true;
    emit downloadFinished(m_savePath, true);
    releaseDownloadResources();
}

void FileDownloader::releaseDownloadResources()
{
    const QString unfinishedPath = m_savePath;
    const bool shouldRemoveIncompleteFile = !m_preserveDownloadedFile && !unfinishedPath.isEmpty();

    if (m_downloadReply) {
        if (!m_downloadReply->isFinished()) {
            m_downloadReply->abort();
        }
        m_downloadReply->deleteLater();
        m_downloadReply = nullptr;
    }

    for (auto it = m_replyToChunkMap.begin(); it != m_replyToChunkMap.end(); ++it) {
        QNetworkReply *reply = it.key();
        if (reply == nullptr) {
            continue;
        }
        if (!reply->isFinished()) {
            reply->abort();
        }
        reply->deleteLater();
    }
    m_replyToChunkMap.clear();
    m_inFlightChunks.clear();

    if (m_targetFile) {
        if (m_targetFile->isOpen()) {
            m_targetFile->close();
        }
        delete m_targetFile;
        m_targetFile = nullptr;
    }

    if (shouldRemoveIncompleteFile && QFile::exists(unfinishedPath)) {
        QFile::remove(unfinishedPath);
    }

    m_savePath.clear();
    m_fileUrl.clear();
    m_totalFileSize = 0;
    m_downloadedBytes = 0;
    m_currentChunkStart = 0;
    m_currentChunkEnd = 0;
    m_isChunkDownload = false;
    m_supportRangeRequest = false;
    m_preserveDownloadedFile = false;
    m_currentChunkIndex = 0;
    m_totalChunks = 0;
}

void FileDownloader::handleRedirect()
{
    if (m_downloadReply == nullptr) {
        return;
    }

    restartDownloadForRedirect(m_downloadReply);
}

void FileDownloader::startConcurrentChunkDownload()
{
    m_inFlightChunks.clear();
    m_replyToChunkMap.clear();
    downloadNextChunks();
}

void FileDownloader::downloadNextChunks()
{
    if (m_currentChunkIndex >= m_totalChunks && m_inFlightChunks.isEmpty()) {
        if (m_targetFile && m_targetFile->isOpen()) {
            m_targetFile->flush();
            m_targetFile->close();
        }

        QNetworkRequest request{QUrl(m_fileUrl)};
        request.setSslConfiguration(m_sslConf);
        request.setRawHeader("User-Agent", "Mozilla/5.0");
        applyAuthorization(request);

        QNetworkReply *headReply = m_netManager->head(request);
        attachSslHandler(headReply);
        connect(headReply, &QNetworkReply::finished, this, [this, headReply]() {
            QString finalPath = finalFilePathFromReply(headReply);
            if (finalPath.isEmpty()) {
                finalPath = m_savePath;
            }

            if (!finalPath.isEmpty() && finalPath != m_savePath && QFile::exists(m_savePath)) {
                const QString desiredPath = uniqueSavePath(finalPath);
                QFile currentFile(m_savePath);
                if (currentFile.rename(desiredPath)) {
                    m_savePath = desiredPath;
                } else if (currentFile.copy(desiredPath)) {
                    currentFile.remove();
                    m_savePath = desiredPath;
                }
            }

            headReply->deleteLater();

            emit downloadProgress(m_totalFileSize, m_totalFileSize);
            if (!m_savePath.isEmpty() && QFile::exists(m_savePath)) {
                const QFileInfo fileInfo(m_savePath);
                QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
            }

            m_preserveDownloadedFile = true;
            emit downloadFinished(m_savePath, true);
            releaseDownloadResources();
        });
        return;
    }

    while (m_inFlightChunks.size() < m_maxConcurrentChunks && m_currentChunkIndex < m_totalChunks) {
        const int chunkIndex = m_currentChunkIndex++;
        const qint64 chunkStart = static_cast<qint64>(chunkIndex) * CHUNK_DOWNLOAD_SIZE;
        const qint64 chunkEnd = qMin(chunkStart + CHUNK_DOWNLOAD_SIZE - 1, m_totalFileSize - 1);

        QNetworkRequest request{QUrl(m_fileUrl)};
        request.setSslConfiguration(m_sslConf);
        request.setRawHeader("User-Agent", "Mozilla/5.0");
        request.setRawHeader("Range", QString("bytes=%1-%2").arg(chunkStart).arg(chunkEnd).toUtf8());
        applyAuthorization(request);

        QNetworkReply *reply = m_netManager->get(request);
        m_inFlightChunks.insert(chunkIndex);
        m_replyToChunkMap.insert(reply, chunkIndex);

        connect(reply, &QNetworkReply::finished, this, &FileDownloader::onConcurrentChunkDownloadFinished);
        attachSslHandler(reply);
    }
}

void FileDownloader::onConcurrentChunkDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    handleConcurrentChunkFinished(reply);
}

void FileDownloader::handleConcurrentChunkFinished(QNetworkReply *reply)
{
    if (reply == nullptr || !m_replyToChunkMap.contains(reply)) {
        if (reply) {
            reply->deleteLater();
        }
        return;
    }

    const int chunkIndex = m_replyToChunkMap.take(reply);
    m_inFlightChunks.remove(chunkIndex);

    if (reply->error() != QNetworkReply::NoError) {
        const QString error = reply->errorString();
        reply->deleteLater();
        emit downloadFailed(error);
        releaseDownloadResources();
        return;
    }

    const QByteArray chunkData = reply->readAll();
    reply->deleteLater();

    if (m_targetFile && m_targetFile->isOpen()) {
        const qint64 writePos = static_cast<qint64>(chunkIndex) * CHUNK_DOWNLOAD_SIZE;
        if (m_targetFile->seek(writePos)) {
            m_targetFile->write(chunkData);
        }
    }

    m_downloadedBytes += chunkData.size();
    emit downloadProgress(m_downloadedBytes, m_totalFileSize);

    downloadNextChunks();
}

void FileDownloader::applyAuthorization(QNetworkRequest &request) const
{
    if (!m_accessToken.isEmpty()) {
        request.setRawHeader("Authorization", QByteArray("Bearer ") + m_accessToken.toUtf8());
    }
}

QUrl FileDownloader::resolvedRedirectUrl(QNetworkReply *reply) const
{
    if (reply == nullptr) {
        return QUrl();
    }

    const QVariant redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!redirect.isValid()) {
        return QUrl();
    }

    QUrl redirectUrl = redirect.toUrl();
    if (redirectUrl.isRelative()) {
        redirectUrl = reply->request().url().resolved(redirectUrl);
    }
    return redirectUrl;
}

bool FileDownloader::restartDownloadForRedirect(QNetworkReply *reply)
{
    const QUrl redirectUrl = resolvedRedirectUrl(reply);
    if (!redirectUrl.isValid() || redirectUrl.isEmpty()) {
        return false;
    }

    const QString userName = m_currentUser;
    const QString token = m_accessToken;
    const QSslConfiguration sslConf = m_sslConf;

    releaseDownloadResources();
    startDownload(redirectUrl.toString(), userName, sslConf, token);
    return true;
}

QString FileDownloader::finalFilePathFromReply(QNetworkReply *reply) const
{
    QString fileName;

    if (reply != nullptr) {
        const QVariant headerValue = reply->header(QNetworkRequest::ContentDispositionHeader);
        const QString disposition = headerValue.toString();
        if (!disposition.isEmpty()) {
            const QRegularExpression utf8Pattern(
                R"(filename\*=UTF-8''([^;]+))",
                QRegularExpression::CaseInsensitiveOption);
            const QRegularExpressionMatch utf8Match = utf8Pattern.match(disposition);
            if (utf8Match.hasMatch()) {
                fileName = QUrl::fromPercentEncoding(utf8Match.captured(1).toUtf8());
            } else {
                const QRegularExpression plainPattern(
                    QStringLiteral("filename=\"?([^\\\";]+)\"?"),
                    QRegularExpression::CaseInsensitiveOption);
                const QRegularExpressionMatch plainMatch = plainPattern.match(disposition);
                if (plainMatch.hasMatch()) {
                    fileName = plainMatch.captured(1).trimmed();
                }
            }
        }
    }

    if (fileName.isEmpty()) {
        return m_savePath;
    }

    fileName.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");
    fileName.remove(QRegularExpression(R"([\x00-\x1F\x7F-\x9F])"));
    if (fileName.isEmpty()) {
        return m_savePath;
    }

    const QFileInfo currentInfo(m_savePath);
    return QDir(currentInfo.absolutePath()).filePath(fileName);
}

QString FileDownloader::uniqueSavePath(const QString &desiredPath) const
{
    if (desiredPath.isEmpty() || !QFile::exists(desiredPath) || desiredPath == m_savePath) {
        return desiredPath;
    }

    const QFileInfo info(desiredPath);
    const QString dirPath = info.absolutePath();
    const QString baseName = info.completeBaseName();
    const QString suffix = info.completeSuffix();

    int index = 1;
    QString candidate;
    do {
        const QString fileName = suffix.isEmpty()
                                     ? QString("%1_%2").arg(baseName).arg(index)
                                     : QString("%1_%2.%3").arg(baseName).arg(index).arg(suffix);
        candidate = QDir(dirPath).filePath(fileName);
        ++index;
    } while (QFile::exists(candidate));

    return candidate;
}
