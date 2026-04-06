#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QDialog>
#include <QList>
#include <QNetworkAccessManager>
#include <QPoint>
#include <QSslConfiguration>
#include <QSslError>
#include <QSslSocket>

class QAbstractItemDelegate;
class QAbstractItemModel;
class QMouseEvent;
class QMessageBox;

namespace Ui {
class CreateGroup;
}

class CreateGroup : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGroup(QNetworkAccessManager *netManager,
                         const QSslConfiguration &sslConf,
                         QSslSocket *mainSslSocket,
                         const QString &currentUser,
                         const QString &accessToken,
                         QWidget *parent = nullptr);
    ~CreateGroup() override;

    void setGroupListModelAndDelegate(QAbstractItemModel *model, QAbstractItemDelegate *delegate);
    void setTargetWindowPos(const QPoint &btnScreenPos);

private slots:
    void on_groupclose_clicked();
    void doMoveToTargetPos();
    void selectAvatar();
    void handleCreateGroup();
    void onSslErrors(const QList<QSslError> &errors);
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onRecvMainWindowSocketData(const QByteArray &socketData);

private:
    QSslConfiguration getSslConfig();
    QString uploadAvatar(const QString &filePath);
    QList<QSslError> ignoreCriticalSslErrors(const QList<QSslError> &errors, QString &errorSummary);
    void sendCreateGroupMessage();
    void closeLoadingMessage();
    QByteArray decodeWebSocketPayload(const QByteArray &socketData) const;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    Ui::CreateGroup *ui;
    QPoint m_targetPos;
    QPoint m_dragPos;
    bool m_isDragging;
    QNetworkAccessManager *m_netManager;
    QSslConfiguration m_sslConf;
    QSslSocket *m_mainSslSocket;
    QString m_currentUser;
    QString m_accessToken;
    bool m_webSocketHandshakeDone;
    QString m_avatarFilePath;
    QString m_avatarURL;
    QMessageBox *m_loadingMsg;
    bool m_createGroupSuccess;
};

#endif // CREATEGROUP_H
