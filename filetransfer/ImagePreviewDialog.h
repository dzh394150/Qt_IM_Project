#ifndef IMAGEPREVIEWDIALOG_H
#define IMAGEPREVIEWDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QSslConfiguration>

class QMouseEvent;

namespace Ui {
class ImagePreviewDialog;
}

class ImagePreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImagePreviewDialog(QWidget *parent = nullptr);
    ~ImagePreviewDialog() override;

    void setImageUrl(const QString &imageUrl,
                     const QSslConfiguration &sslConf,
                     const QString &accessToken = QString());

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onImageDownloadFinished(QNetworkReply *reply);
    void onBtnCloseClicked();

private:
    QString getCachedImagePath(const QString &imageUrl) const;
    void loadCachedImage(const QString &cachePath);
    void drawPreviewImage(const QPixmap &pixmap);

    Ui::ImagePreviewDialog *ui;
    QNetworkAccessManager *m_netManager;
    QSslConfiguration m_sslConf;
    QString m_targetImageUrl;
    QString m_accessToken;
    QPixmap m_previewPixmap;
    bool m_isDragging;
    QPoint m_dragStartPosition;
};

#endif // IMAGEPREVIEWDIALOG_H
