#include "ImagePreviewDialog.h"
#include "ui_ImagePreviewDialog.h"

#include "CacheUtils.h"

#include <QCryptographicHash>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkRequest>
#include <QPixmap>
#include <QUrl>

ImagePreviewDialog::ImagePreviewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImagePreviewDialog)
    , m_netManager(new QNetworkAccessManager(this))
    , m_isDragging(false)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    auto *shadow = new QGraphicsDropShadowEffect(ui->shadowwidget);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 0);
    ui->shadowwidget->setGraphicsEffect(shadow);

    connect(ui->btnclose, &QPushButton::clicked, this, &ImagePreviewDialog::onBtnCloseClicked);
}

ImagePreviewDialog::~ImagePreviewDialog()
{
    delete ui;
}

void ImagePreviewDialog::setImageUrl(const QString &imageUrl,
                                     const QSslConfiguration &sslConf,
                                     const QString &accessToken)
{
    if (imageUrl.isEmpty()) {
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setText(QStringLiteral("图片地址为空，无法预览"));
        return;
    }

    m_targetImageUrl = imageUrl;
    m_sslConf = sslConf;
    m_accessToken = accessToken;

    const QString cachePath = getCachedImagePath(imageUrl);
    if (QFile::exists(cachePath)) {
        loadCachedImage(cachePath);
        return;
    }

    QNetworkRequest request{QUrl(imageUrl)};
    request.setSslConfiguration(m_sslConf);
    if (!m_accessToken.isEmpty()) {
        request.setRawHeader("Authorization", QByteArray("Bearer ") + m_accessToken.toUtf8());
    }

    QNetworkReply *reply = m_netManager->get(request);
    connect(reply, &QNetworkReply::sslErrors, this, [reply](const QList<QSslError> &errors) {
        Q_UNUSED(errors);
        reply->ignoreSslErrors();
    });
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onImageDownloadFinished(reply);
    });

    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setText(QStringLiteral("正在加载图片..."));
}

QString ImagePreviewDialog::getCachedImagePath(const QString &imageUrl) const
{
    const QByteArray hash = QCryptographicHash::hash(imageUrl.toUtf8(), QCryptographicHash::Md5);
    return QString("%1/%2.png").arg(CacheUtils::getFilePreviewCacheDir(), QString::fromLatin1(hash.toHex()));
}

void ImagePreviewDialog::loadCachedImage(const QString &cachePath)
{
    QPixmap pixmap;
    if (!pixmap.load(cachePath)) {
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setText(QStringLiteral("本地缓存图片损坏，无法预览"));
        return;
    }

    drawPreviewImage(pixmap);
}

void ImagePreviewDialog::drawPreviewImage(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setText(QStringLiteral("图片加载失败"));
        return;
    }

    m_previewPixmap = pixmap;
    ui->imageLabel->setPixmap(
        pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->imageLabel->setAlignment(Qt::AlignCenter);
}

void ImagePreviewDialog::onImageDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setText(QStringLiteral("图片加载失败: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    const QByteArray imageData = reply->readAll();
    QPixmap pixmap;
    if (!pixmap.loadFromData(imageData)) {
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setText(QStringLiteral("图片数据损坏，无法预览"));
        reply->deleteLater();
        return;
    }

    const QString cachePath = getCachedImagePath(m_targetImageUrl);
    pixmap.save(cachePath, "PNG");
    drawPreviewImage(pixmap);
    reply->deleteLater();
}

void ImagePreviewDialog::onBtnCloseClicked()
{
    m_previewPixmap = QPixmap();
    ui->imageLabel->clear();
    close();
}

void ImagePreviewDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const int dragAreaHeight = 35;
        const int closeButtonWidth = 50;
        if (event->pos().y() < dragAreaHeight && event->pos().x() < width() - closeButtonWidth) {
            m_isDragging = true;
            m_dragStartPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
            return;
        }
    }

    QDialog::mousePressEvent(event);
}

void ImagePreviewDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragStartPosition);
        event->accept();
        return;
    }

    QDialog::mouseMoveEvent(event);
}

void ImagePreviewDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        event->accept();
    }

    QDialog::mouseReleaseEvent(event);
}
