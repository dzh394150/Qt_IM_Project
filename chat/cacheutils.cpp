#include "CacheUtils.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

// 辅助方法：创建目录（递归创建，返回是否创建成功/目录已存在）
bool CacheUtils::createDirIfNotExists(const QString &dirPath)
{
    QDir dir(dirPath);
    if (dir.exists()) {
        return true; // 目录已存在，无需创建
    }

    // 递归创建目录（父目录不存在也会自动创建）
    bool isCreated = dir.mkpath(dirPath);
    if (!isCreated) {
        qDebug() << "【缓存目录创建失败】路径：" << dirPath << "（无写入权限或路径非法）";
    } else {
        qDebug() << "【缓存目录创建成功】路径：" << dirPath;
    }
    return isCreated;
}

// 获取头像缓存目录（自动创建）
QString CacheUtils::getAvatarCacheDir()
{
    // 选择系统可写目录（避免权限问题），拼接自定义缓存路径
    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString avatarDirPath = baseDir + "/ChatCache/Avatar";

    // 自动创建目录
    createDirIfNotExists(avatarDirPath);

    return avatarDirPath;
}

// 获取文件预览图缓存目录（自动创建）
QString CacheUtils::getFilePreviewCacheDir()
{
    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString previewDirPath = baseDir + "/ChatCache/Preview";

    // 自动创建目录
    createDirIfNotExists(previewDirPath);

    return previewDirPath;
}

// 【补充：获取文件下载根目录（贴合你的Windows端需求，预留跨平台适配）】
QString CacheUtils::getFileDownloadRootDir(const QString &userName)
{
    if (userName.isEmpty()) {
        qDebug() << "【文件下载根目录】用户名为空，无法构建路径";
        return QString();
    }

    // ====== Windows端：运行目录/download_file_用户名/ ======
    // QDir::currentPath()：获取应用运行目录（你需要的Windows端路径）
    QString appRunDir = QDir::currentPath();
    // 拼接下载根目录路径（格式：download_file_用户名）
    QString downloadRootDir = appRunDir + QString("/download_file_%1/").arg(userName);

    // ====== 预留跨平台适配（后续扩展Linux/macOS）======
    // 注释：如需适配Linux/macOS，可在此处添加条件判断
    // if (QSysInfo::operatingSystem() == QSysInfo::Linux) {
    //     downloadRootDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QString("/ChatDownload_%1/").arg(userName);
    // } else if (QSysInfo::operatingSystem() == QSysInfo::macOS) {
    //     downloadRootDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QString("/ChatDownload_%1/").arg(userName);
    // }

    // 注：此处仅返回路径，不创建目录（创建目录的职责交给 FileDownloader::createDownloadDir）
    // 符合你的现有逻辑：createDownloadDir 中调用 dir.mkpath(".") 创建目录
    qDebug() << "【文件下载根目录】构建完成：" << downloadRootDir;
    return downloadRootDir;
}
