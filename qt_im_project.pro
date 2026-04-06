# 核心模块（core+gui+widgets是GUI项目基础，websockets用于通信，sql用于本地数据库）
QT       += core gui widgets network websockets sql

# QT版本兼容（QT6自动满足，保留仅为兼容写法）
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# C++版本
CONFIG += c++17
QT_CONFIG -= no-pkg-config
# 禁用Qt扩展警告（可选，减少编译提示）
DEFINES += QT_DEPRECATED_WARNINGS

# 添加头文件搜索路径（包含所有子模块文件夹）
INCLUDEPATH += \
    core \
    auth \
    chat \
    contacts \
    storage \
    filetransfer

# 源文件列表
SOURCES += \
    core/main.cpp \
    core/mainwindow.cpp \
    auth/login.cpp \
    auth/registerwidget.cpp \
    chat/chatmodel.cpp \
    chat/chatdelegate.cpp \
    chat/cacheutils.cpp \
    contacts/AddFriends.cpp \
    contacts/CreateGroup.cpp \
    contacts/FriendListDelegate.cpp \
    contacts/FriendListModel.cpp \
    contacts/FriendRequestDelegate.cpp \
    contacts/FriendRequestModel.cpp \
    contacts/GroupListDelegate.cpp \
    contacts/GroupListModel.cpp \
    storage/LocalChatCache.cpp \
    filetransfer/FileDownloader.cpp \
    filetransfer/FileTransferWorker.cpp \
    filetransfer/FileUtils.cpp \
    filetransfer/ImagePreviewDialog.cpp

# 头文件列表
HEADERS += \
    core/mainwindow.h \
    auth/login.h \
    auth/registerwidget.h \
    chat/chatmodel.h \
    chat/chatdelegate.h \
    chat/chatmessage.h \
    chat/cacheutils.h \
    contacts/AddFriends.h \
    contacts/CreateGroup.h \
    contacts/FriendListDelegate.h \
    contacts/FriendListModel.h \
    contacts/FriendRequestDelegate.h \
    contacts/FriendRequestModel.h \
    contacts/GroupListDelegate.h \
    contacts/GroupListModel.h \
    storage/LocalChatCache.h \
    filetransfer/FileAttachment.h \
    filetransfer/FileChunkInfo.h \
    filetransfer/FileDownloader.h \
    filetransfer/FileTransferWorker.h \
    filetransfer/FileUtils.h \
    filetransfer/ImagePreviewDialog.h

# UI界面文件列表
FORMS += \
    core/mainwindow.ui \
    auth/login.ui \
    auth/registerwidget.ui \
    contacts/AddFriends.ui \
    contacts/CreateGroup.ui \
    filetransfer/ImagePreviewDialog.ui

# 部署规则（默认保留）
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
