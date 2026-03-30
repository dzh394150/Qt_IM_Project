/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17ChatInputTextEditE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatInputTextEdit::qt_create_metaobjectdata<qt_meta_tag_ZN17ChatInputTextEditE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatInputTextEdit",
        "fileDropped",
        "",
        "filePath"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'fileDropped'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChatInputTextEdit, qt_meta_tag_ZN17ChatInputTextEditE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatInputTextEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QTextEdit::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatInputTextEditE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatInputTextEditE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ChatInputTextEditE_t>.metaTypes,
    nullptr
} };

void ChatInputTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatInputTextEdit *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->fileDropped((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChatInputTextEdit::*)(const QString & )>(_a, &ChatInputTextEdit::fileDropped, 0))
            return;
    }
}

const QMetaObject *ChatInputTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatInputTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatInputTextEditE_t>.strings))
        return static_cast<void*>(this);
    return QTextEdit::qt_metacast(_clname);
}

int ChatInputTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ChatInputTextEdit::fileDropped(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "signalGroupSocketDataArrived",
        "",
        "socketData",
        "startProcessChunk",
        "QFile*",
        "file",
        "fileTotalSize",
        "hasTransferred",
        "chunkIndex",
        "totalChunks",
        "fileUniqueId",
        "fileName",
        "fileType",
        "fromUser",
        "toUser",
        "showChatGroupChatRedDot",
        "chatUnreadCount",
        "groupChatUnreadCount",
        "onSslReadyRead",
        "onWebSocketDisconnected",
        "onPersonListItemClicked",
        "QModelIndex",
        "index",
        "onFriendRequestListItemClicked",
        "onSendButtonClicked",
        "onBtnLookFriendClicked",
        "onBtnAgreeClicked",
        "onBtnRefuseClicked",
        "onSslErrors",
        "QNetworkReply*",
        "reply",
        "QList<QSslError>",
        "errors",
        "toggleMaximize",
        "onBtnAddFriendsClicked",
        "onBtnStartChatClicked",
        "loadUserAvatar",
        "avatarUrl",
        "onAvatarLoaded",
        "showListViewScrollBar",
        "QListView*",
        "listView",
        "show",
        "onChatViewScrollValueChanged",
        "value",
        "onFileDropped",
        "filePath",
        "on_chatInput_textChanged",
        "onChatViewClicked",
        "onFileDownloadProgressUpdated",
        "bytesReceived",
        "bytesTotal",
        "onFileDownloadFinished",
        "savePath",
        "isSuccess",
        "onFileDownloadFailed",
        "errorMsg",
        "resizeEvent",
        "QResizeEvent*",
        "event",
        "focusOutEvent",
        "QFocusEvent*",
        "eventFilter",
        "watched",
        "QEvent*",
        "onChatViewLeave",
        "onAsyncSendNextChunk",
        "resetFileSendState",
        "onBytesWritten",
        "bytes",
        "onChunkTimeout",
        "onChunkProcessed",
        "FileChunkInfo",
        "chunkInfo",
        "webSocketFrame",
        "onProcessError",
        "errorMessage",
        "ongroupchatClicked",
        "on_btnCreateGroup_clicked",
        "onGroupListItemClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalGroupSocketDataArrived'
        QtMocHelpers::SignalData<void(const QByteArray &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Signal 'startProcessChunk'
        QtMocHelpers::SignalData<void(QFile *, qint64, qint64, int, int, const QString &, const QString &, const QString &, const QString &, const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 }, { QMetaType::LongLong, 7 }, { QMetaType::LongLong, 8 }, { QMetaType::Int, 9 },
            { QMetaType::Int, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
            { QMetaType::QString, 14 }, { QMetaType::QString, 15 },
        }}),
        // Slot 'showChatGroupChatRedDot'
        QtMocHelpers::SlotData<void(int, int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 }, { QMetaType::Int, 18 },
        }}),
        // Slot 'onSslReadyRead'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onWebSocketDisconnected'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPersonListItemClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Slot 'onFriendRequestListItemClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Slot 'onSendButtonClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBtnLookFriendClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBtnAgreeClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBtnRefuseClicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSslErrors'
        QtMocHelpers::SlotData<void(QNetworkReply *, const QList<QSslError> &)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 30, 31 }, { 0x80000000 | 32, 33 },
        }}),
        // Slot 'toggleMaximize'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBtnAddFriendsClicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBtnStartChatClicked'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadUserAvatar'
        QtMocHelpers::SlotData<void(const QString &)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 38 },
        }}),
        // Slot 'onAvatarLoaded'
        QtMocHelpers::SlotData<void(QNetworkReply *)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
        // Slot 'showListViewScrollBar'
        QtMocHelpers::SlotData<void(QListView *, bool)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 41, 42 }, { QMetaType::Bool, 43 },
        }}),
        // Slot 'onChatViewScrollValueChanged'
        QtMocHelpers::SlotData<void(int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 45 },
        }}),
        // Slot 'onFileDropped'
        QtMocHelpers::SlotData<void(const QString &)>(46, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 47 },
        }}),
        // Slot 'on_chatInput_textChanged'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onChatViewClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Slot 'onFileDownloadProgressUpdated'
        QtMocHelpers::SlotData<void(qint64, qint64)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 51 }, { QMetaType::LongLong, 52 },
        }}),
        // Slot 'onFileDownloadFinished'
        QtMocHelpers::SlotData<void(const QString &, bool)>(53, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 54 }, { QMetaType::Bool, 55 },
        }}),
        // Slot 'onFileDownloadFailed'
        QtMocHelpers::SlotData<void(const QString &)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 57 },
        }}),
        // Slot 'resizeEvent'
        QtMocHelpers::SlotData<void(QResizeEvent *)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 59, 60 },
        }}),
        // Slot 'focusOutEvent'
        QtMocHelpers::SlotData<void(QFocusEvent *)>(61, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 62, 60 },
        }}),
        // Slot 'eventFilter'
        QtMocHelpers::SlotData<bool(QObject *, QEvent *)>(63, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QObjectStar, 64 }, { 0x80000000 | 65, 60 },
        }}),
        // Slot 'onChatViewLeave'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAsyncSendNextChunk'
        QtMocHelpers::SlotData<void()>(67, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resetFileSendState'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBytesWritten'
        QtMocHelpers::SlotData<void(qint64)>(69, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 70 },
        }}),
        // Slot 'onChunkTimeout'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onChunkTimeout'
        QtMocHelpers::SlotData<void(int)>(71, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'onChunkProcessed'
        QtMocHelpers::SlotData<void(const FileChunkInfo &, const QByteArray &)>(72, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 73, 74 }, { QMetaType::QByteArray, 75 },
        }}),
        // Slot 'onProcessError'
        QtMocHelpers::SlotData<void(const QString &)>(76, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 77 },
        }}),
        // Slot 'ongroupchatClicked'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnCreateGroup_clicked'
        QtMocHelpers::SlotData<void()>(79, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGroupListItemClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(80, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalGroupSocketDataArrived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->startProcessChunk((*reinterpret_cast<std::add_pointer_t<QFile*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[9])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[10]))); break;
        case 2: _t->showChatGroupChatRedDot((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->onSslReadyRead(); break;
        case 4: _t->onWebSocketDisconnected(); break;
        case 5: _t->onPersonListItemClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 6: _t->onFriendRequestListItemClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 7: _t->onSendButtonClicked(); break;
        case 8: _t->onBtnLookFriendClicked(); break;
        case 9: _t->onBtnAgreeClicked(); break;
        case 10: _t->onBtnRefuseClicked(); break;
        case 11: _t->onSslErrors((*reinterpret_cast<std::add_pointer_t<QNetworkReply*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<QSslError>>>(_a[2]))); break;
        case 12: _t->toggleMaximize(); break;
        case 13: _t->onBtnAddFriendsClicked(); break;
        case 14: _t->onBtnStartChatClicked(); break;
        case 15: _t->loadUserAvatar((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->onAvatarLoaded((*reinterpret_cast<std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 17: _t->showListViewScrollBar((*reinterpret_cast<std::add_pointer_t<QListView*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 18: _t->onChatViewScrollValueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->onFileDropped((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->on_chatInput_textChanged(); break;
        case 21: _t->onChatViewClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 22: _t->onFileDownloadProgressUpdated((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2]))); break;
        case 23: _t->onFileDownloadFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 24: _t->onFileDownloadFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->resizeEvent((*reinterpret_cast<std::add_pointer_t<QResizeEvent*>>(_a[1]))); break;
        case 26: _t->focusOutEvent((*reinterpret_cast<std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        case 27: { bool _r = _t->eventFilter((*reinterpret_cast<std::add_pointer_t<QObject*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QEvent*>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 28: _t->onChatViewLeave(); break;
        case 29: _t->onAsyncSendNextChunk(); break;
        case 30: _t->resetFileSendState(); break;
        case 31: _t->onBytesWritten((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 32: _t->onChunkTimeout(); break;
        case 33: _t->onChunkTimeout((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->onChunkProcessed((*reinterpret_cast<std::add_pointer_t<FileChunkInfo>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 35: _t->onProcessError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 36: _t->ongroupchatClicked(); break;
        case 37: _t->on_btnCreateGroup_clicked(); break;
        case 38: _t->onGroupListItemClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QFile* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QSslError> >(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QListView* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(const QByteArray & )>(_a, &MainWindow::signalGroupSocketDataArrived, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(QFile * , qint64 , qint64 , int , int , const QString & , const QString & , const QString & , const QString & , const QString & )>(_a, &MainWindow::startProcessChunk, 1))
            return;
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::signalGroupSocketDataArrived(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MainWindow::startProcessChunk(QFile * _t1, qint64 _t2, qint64 _t3, int _t4, int _t5, const QString & _t6, const QString & _t7, const QString & _t8, const QString & _t9, const QString & _t10)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10);
}
QT_WARNING_POP
