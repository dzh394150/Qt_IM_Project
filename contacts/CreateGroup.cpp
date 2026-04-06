#include "CreateGroup.h"
#include "ui_CreateGroup.h"

#include "FriendListModel.h"
#include "mainwindow.h"

#include <QAbstractItemView>
#include <QDateTime>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QItemSelectionModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRandomGenerator>
#include <QTimer>

CreateGroup::CreateGroup(QNetworkAccessManager *netManager,
                         const QSslConfiguration &sslConf,
                         QSslSocket *mainSslSocket,
                         const QString &currentUser,
                         const QString &accessToken,
                         QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateGroup)
    , m_isDragging(false)
    , m_netManager(netManager)
    , m_sslConf(sslConf)
    , m_mainSslSocket(mainSslSocket)
    , m_currentUser(currentUser)
    , m_accessToken(accessToken)
    , m_webSocketHandshakeDone(true)
    , m_loadingMsg(nullptr)
    , m_createGroupSuccess(false)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->creategroupList->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->btnAvatargroup, &QPushButton::clicked, this, &CreateGroup::selectAvatar);
    connect(ui->btncreategroup, &QPushButton::clicked, this, &CreateGroup::handleCreateGroup);
    connect(ui->groupclose, &QPushButton::clicked, this, &CreateGroup::on_groupclose_clicked);

    if (m_mainSslSocket) {
        connect(m_mainSslSocket, &QSslSocket::sslErrors, this, &CreateGroup::onSslErrors);
        connect(m_mainSslSocket, &QSslSocket::disconnected, this, &CreateGroup::onDisconnected);
        connect(m_mainSslSocket,
                QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::errorOccurred),
                this,
                &CreateGroup::onSocketError);
    }

    if (auto *mainWindow = qobject_cast<MainWindow *>(parent)) {
        connect(mainWindow,
                &MainWindow::signalGroupSocketDataArrived,
                this,
                &CreateGroup::onRecvMainWindowSocketData,
                Qt::QueuedConnection);
    }

    layout()->activate();
    adjustSize();
    if (parent) {
        const QRect parentRect = parent->geometry();
        const QRect selfRect = geometry();
        move(parentRect.x() + (parentRect.width() - selfRect.width()) / 2,
             parentRect.y() + (parentRect.height() - selfRect.height()) / 2);
    }
}

CreateGroup::~CreateGroup()
{
    closeLoadingMessage();
    delete ui;
}

void CreateGroup::setGroupListModelAndDelegate(QAbstractItemModel *model, QAbstractItemDelegate *delegate)
{
    if (!model || !delegate) {
        return;
    }

    ui->creategroupList->setModel(model);
    ui->creategroupList->setItemDelegate(delegate);
}

void CreateGroup::setTargetWindowPos(const QPoint &btnScreenPos)
{
    m_targetPos = btnScreenPos;
    QTimer::singleShot(0, this, &CreateGroup::doMoveToTargetPos);
}

void CreateGroup::doMoveToTargetPos()
{
    adjustSize();
    move(m_targetPos);
}

QSslConfiguration CreateGroup::getSslConfig()
{
    return m_sslConf;
}

void CreateGroup::selectAvatar()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("选择群头像"),
        QString(),
        QStringLiteral("Images (*.png *.jpg *.jpeg *.gif)"));
    if (filePath.isEmpty()) {
        return;
    }

    m_avatarFilePath = filePath;
    ui->btnAvatargroup->setText(QFileInfo(filePath).fileName());
}

QString CreateGroup::uploadAvatar(const QString &filePath)
{
    if (filePath.isEmpty() || m_netManager == nullptr) {
        return QString();
    }

    auto *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        delete file;
        QMessageBox::warning(this, "提示", "群头像文件读取失败。");
        return QString();
    }

    QNetworkRequest request(QUrl("https://www.singchat.chat/upload/avatar"));
    request.setTransferTimeout(10000);
    request.setSslConfiguration(getSslConfig());
    request.setRawHeader("User-Agent", "curl/7.29.0");
    request.setRawHeader("Host", "www.singchat.chat");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Expect", "");
    if (!m_accessToken.isEmpty()) {
        request.setRawHeader("Authorization", QByteArray("Bearer ") + m_accessToken.toUtf8());
    }

    auto *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;

    const QFileInfo fileInfo(filePath);
    const QString suffix = fileInfo.suffix().toLower();
    QString mimeType = "image/jpeg";
    if (suffix == "png") {
        mimeType = "image/png";
    } else if (suffix == "gif") {
        mimeType = "image/gif";
    }

    const QString safeFileName =
        QString("group_avatar_%1.%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(suffix);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(QString("form-data; name=\"avatar\"; filename=\"%1\"")
                                     .arg(safeFileName)));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType));
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    QNetworkReply *reply = m_netManager->post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::sslErrors, this, [this, reply](const QList<QSslError> &errors) {
        QString errorSummary;
        const QList<QSslError> ignoreErrors = ignoreCriticalSslErrors(errors, errorSummary);
        if (!ignoreErrors.isEmpty()) {
            reply->ignoreSslErrors(ignoreErrors);
            return;
        }

        QMessageBox::critical(this, "SSL 验证失败", "群头像上传已终止。\n" + errorSummary);
        reply->abort();
    });

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString avatarUrl;
    if (reply->error() == QNetworkReply::NoError) {
        avatarUrl = QString::fromUtf8(reply->readAll()).trimmed();
    } else {
        QMessageBox::warning(
            this,
            "提示",
            QString("群头像上传失败：%1").arg(reply->errorString()));
    }

    reply->deleteLater();
    return avatarUrl;
}

void CreateGroup::handleCreateGroup()
{
    m_createGroupSuccess = false;
    closeLoadingMessage();

    const QString groupName = ui->groupname->text().trimmed();
    QItemSelectionModel *selectionModel = ui->creategroupList->selectionModel();
    const QModelIndexList selectedIndexes =
        selectionModel ? selectionModel->selectedIndexes() : QModelIndexList();

    if (groupName.isEmpty()) {
        QMessageBox::warning(this, "提示", "群聊名称不能为空。");
        ui->groupname->setFocus();
        return;
    }

    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "提示", "请至少选择一位好友加入群聊。");
        return;
    }

    if (m_avatarFilePath.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择群头像。");
        return;
    }

    auto *uploadMsg =
        new QMessageBox(QMessageBox::Information, "提示", "正在上传群头像...", QMessageBox::NoButton, this);
    uploadMsg->show();
    m_avatarURL = uploadAvatar(m_avatarFilePath);
    uploadMsg->close();
    uploadMsg->deleteLater();

    if (m_avatarURL.isEmpty()) {
        return;
    }

    m_loadingMsg = new QMessageBox(
        QMessageBox::Information, "提示", "正在创建群聊...", QMessageBox::NoButton, this);
    m_loadingMsg->show();
    sendCreateGroupMessage();
}

void CreateGroup::sendCreateGroupMessage()
{
    if (!m_mainSslSocket || m_mainSslSocket->state() != QSslSocket::ConnectedState ||
        !m_mainSslSocket->isEncrypted()) {
        QMessageBox::critical(this, "发送失败", "SSL 连接未就绪，无法发送创建群聊请求。");
        closeLoadingMessage();
        return;
    }

    QItemSelectionModel *selectionModel = ui->creategroupList->selectionModel();
    const QModelIndexList selectedIndexes =
        selectionModel ? selectionModel->selectedIndexes() : QModelIndexList();

    QJsonArray membersArray;
    membersArray.append(m_currentUser);
    for (const QModelIndex &index : selectedIndexes) {
        const QString username = index.data(FriendListModel::UsernameRole).toString().trimmed();
        if (!username.isEmpty() && username != m_currentUser && !membersArray.contains(username)) {
            membersArray.append(username);
        }
    }

    QJsonObject groupMsg;
    groupMsg["type"] = "create_group";
    groupMsg["creator"] = m_currentUser;
    groupMsg["display_name"] = ui->groupname->text().trimmed();
    groupMsg["members"] = membersArray;
    groupMsg["avatar"] = m_avatarURL;
    groupMsg["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QByteArray payload = QJsonDocument(groupMsg).toJson(QJsonDocument::Compact);
    QByteArray maskingKey(4, 0);
    QRandomGenerator::system()->fillRange(reinterpret_cast<quint32 *>(maskingKey.data()), 1);

    QByteArray frame;
    frame.append(char(0x81));
    if (payload.size() <= 125) {
        frame.append(static_cast<char>(payload.size() | 0x80));
    } else if (payload.size() <= 65535) {
        frame.append(char(0xFE));
        frame.append(char((payload.size() >> 8) & 0xFF));
        frame.append(char(payload.size() & 0xFF));
    } else {
        QMessageBox::warning(this, "提示", "群聊创建请求过大，发送失败。");
        closeLoadingMessage();
        return;
    }

    frame.append(maskingKey);
    for (int i = 0; i < payload.size(); ++i) {
        payload[i] = char(payload[i] ^ maskingKey[i % 4]);
    }
    frame.append(payload);

    m_mainSslSocket->write(frame);
    m_mainSslSocket->flush();
}

void CreateGroup::onRecvMainWindowSocketData(const QByteArray &socketData)
{
    if (!m_webSocketHandshakeDone || socketData.isEmpty()) {
        return;
    }

    const QByteArray payload = decodeWebSocketPayload(socketData);
    if (payload.isEmpty()) {
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject()) {
        return;
    }

    const QJsonObject obj = doc.object();
    const QString type = obj.value("type").toString();
    const QString content = obj.value("content").toString();
    const QString groupId = obj.value("group_id").toString();

    if (type != "tip") {
        return;
    }

    if (content.contains(QStringLiteral("创建成功"))) {
        m_createGroupSuccess = true;
        closeLoadingMessage();
        QMessageBox::information(
            this,
            "创建结果",
            groupId.isEmpty() ? content : QString("%1\n群聊 ID：%2").arg(content, groupId));
        close();
        return;
    }

    if (m_loadingMsg != nullptr && content.contains(QStringLiteral("群"))) {
        closeLoadingMessage();
        QMessageBox::warning(this, "提示", content);
    }
}

QList<QSslError> CreateGroup::ignoreCriticalSslErrors(const QList<QSslError> &errors,
                                                      QString &errorSummary)
{
    QList<QSslError> ignoreErrors;
    QStringList summaries;
    for (const QSslError &error : errors) {
        summaries << error.errorString();
        if (error.error() == QSslError::HostNameMismatch ||
            error.error() == QSslError::SelfSignedCertificate ||
            error.error() == QSslError::CertificateUntrusted) {
            ignoreErrors << error;
        }
    }
    errorSummary = summaries.join('\n');
    return ignoreErrors;
}

void CreateGroup::onSslErrors(const QList<QSslError> &errors)
{
    if (!m_mainSslSocket) {
        return;
    }

    QString errorSummary;
    const QList<QSslError> ignoreErrors = ignoreCriticalSslErrors(errors, errorSummary);
    if (!ignoreErrors.isEmpty()) {
        m_mainSslSocket->ignoreSslErrors(ignoreErrors);
    }
}

void CreateGroup::onDisconnected()
{
    if (!m_createGroupSuccess) {
        closeLoadingMessage();
        QMessageBox::warning(this, "提示", "连接已断开，群聊创建未完成。");
    }
}

void CreateGroup::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    if (!m_createGroupSuccess) {
        closeLoadingMessage();
        QMessageBox::warning(
            this,
            "提示",
            m_mainSslSocket ? m_mainSslSocket->errorString() : QStringLiteral("连接异常"));
    }
}

void CreateGroup::closeLoadingMessage()
{
    if (m_loadingMsg) {
        m_loadingMsg->close();
        m_loadingMsg->deleteLater();
        m_loadingMsg = nullptr;
    }
}

QByteArray CreateGroup::decodeWebSocketPayload(const QByteArray &socketData) const
{
    if (socketData.size() < 2) {
        return QByteArray();
    }

    quint64 payloadLen = quint8(socketData[1]) & 0x7F;
    int offset = 2;
    if (payloadLen == 126) {
        if (socketData.size() < 4) {
            return QByteArray();
        }
        payloadLen = (quint8(socketData[2]) << 8) | quint8(socketData[3]);
        offset = 4;
    } else if (payloadLen == 127) {
        if (socketData.size() < 10) {
            return QByteArray();
        }
        payloadLen = 0;
        for (int i = 0; i < 8; ++i) {
            payloadLen = (payloadLen << 8) | quint8(socketData[2 + i]);
        }
        offset = 10;
    }

    QByteArray maskKey;
    if ((quint8(socketData[1]) & 0x80) != 0) {
        if (socketData.size() < offset + 4) {
            return QByteArray();
        }
        maskKey = socketData.mid(offset, 4);
        offset += 4;
    }

    if (socketData.size() < offset + static_cast<int>(payloadLen)) {
        return QByteArray();
    }

    QByteArray payload = socketData.mid(offset, payloadLen);
    if (!maskKey.isEmpty()) {
        for (int i = 0; i < payload.size(); ++i) {
            payload[i] = char(payload[i] ^ maskKey[i % 4]);
        }
    }

    return payload;
}

void CreateGroup::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
        return;
    }

    QDialog::mousePressEvent(event);
}

void CreateGroup::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPos);
        event->accept();
        return;
    }

    QDialog::mouseMoveEvent(event);
}

void CreateGroup::on_groupclose_clicked()
{
    closeLoadingMessage();
    close();
}
