#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QSet>
#include <QString>

const qint64 CHUNK_DOWNLOAD_SIZE = 2 * 1024 * 1024;

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QObject *parent = nullptr);
    ~FileDownloader() override;

    void startDownload(const QString &downloadUrl,
                       const QString &userName,
                       const QSslConfiguration &sslConf,
                       const QString &accessToken);

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(const QString &savePath, bool isSuccess);
    void downloadFailed(const QString &errorMsg);

private slots:
    void onDownloadReadyRead();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();
    void handleRedirect();
    void onChunkDownloadFinished();
    void onConcurrentChunkDownloadFinished();

private:
    QString createDownloadDir(const QString &userName);
    QString extractFileName(const QString &downloadUrl) const;
    void releaseDownloadResources();

    void startChunkDownload(const QString &downloadUrl,
                            const QString &userName,
                            const QSslConfiguration &sslConf);
    void downloadNextChunk();
    void onFileSizeReceived();
    void startConcurrentChunkDownload();
    void downloadNextChunks();
    void handleConcurrentChunkFinished(QNetworkReply *reply);
    void applyAuthorization(QNetworkRequest &request) const;

    QUrl resolvedRedirectUrl(QNetworkReply *reply) const;
    bool restartDownloadForRedirect(QNetworkReply *reply);
    void startDirectDownload();
    QString finalFilePathFromReply(QNetworkReply *reply) const;
    QString uniqueSavePath(const QString &desiredPath) const;

    QNetworkAccessManager *m_netManager;
    QSslConfiguration m_sslConf;
    QNetworkReply *m_downloadReply;
    QFile *m_targetFile;
    QString m_savePath;
    QString m_currentUser;
    QString m_accessToken;
    QString m_fileUrl;
    qint64 m_totalFileSize;
    qint64 m_downloadedBytes;
    qint64 m_currentChunkStart;
    qint64 m_currentChunkEnd;
    bool m_isChunkDownload;
    bool m_supportRangeRequest;
    bool m_preserveDownloadedFile;

    const int m_maxConcurrentChunks = 8;
    int m_currentChunkIndex;
    int m_totalChunks;
    QSet<int> m_inFlightChunks;
    QMap<QNetworkReply *, int> m_replyToChunkMap;
};

#endif // FILEDOWNLOADER_H
