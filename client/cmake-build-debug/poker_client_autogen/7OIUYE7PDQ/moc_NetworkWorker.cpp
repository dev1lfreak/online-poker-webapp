/****************************************************************************
** Meta object code from reading C++ file 'NetworkWorker.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/net/NetworkWorker.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkWorker.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN6client13NetworkWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto client::NetworkWorker::qt_create_metaobjectdata<qt_meta_tag_ZN6client13NetworkWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "client::NetworkWorker",
        "connected",
        "",
        "disconnected",
        "socketError",
        "error",
        "loginAccepted",
        "playerId",
        "joinedTable",
        "tableId",
        "actionReceived",
        "QJsonObject",
        "event",
        "stateReceived",
        "state",
        "chatReceived",
        "text",
        "rawMessageReceived",
        "message",
        "connectToServer",
        "host",
        "port",
        "disconnectFromServer",
        "sendLogin",
        "nickname",
        "sendJoin",
        "sendJoinWithBots",
        "sendStartGame",
        "sendLeave",
        "sendDisconnect",
        "sendCheck",
        "sendCall",
        "sendFold",
        "sendRaise",
        "amount",
        "onReadyRead",
        "onConnected",
        "onDisconnected",
        "onError",
        "QAbstractSocket::SocketError"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connected'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'disconnected'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'socketError'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'loginAccepted'
        QtMocHelpers::SignalData<void(quint64)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::ULongLong, 7 },
        }}),
        // Signal 'joinedTable'
        QtMocHelpers::SignalData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'actionReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Signal 'stateReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 14 },
        }}),
        // Signal 'chatReceived'
        QtMocHelpers::SignalData<void(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 },
        }}),
        // Signal 'rawMessageReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 18 },
        }}),
        // Slot 'connectToServer'
        QtMocHelpers::SlotData<void(const QString &, quint16)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 }, { QMetaType::UShort, 21 },
        }}),
        // Slot 'disconnectFromServer'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendLogin'
        QtMocHelpers::SlotData<void(const QString &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 },
        }}),
        // Slot 'sendJoin'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendJoinWithBots'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendStartGame'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendLeave'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendDisconnect'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendCheck'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendCall'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendFold'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendRaise'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 34 },
        }}),
        // Slot 'onReadyRead'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onError'
        QtMocHelpers::SlotData<void(QAbstractSocket::SocketError)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 39, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NetworkWorker, qt_meta_tag_ZN6client13NetworkWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject client::NetworkWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client13NetworkWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client13NetworkWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6client13NetworkWorkerE_t>.metaTypes,
    nullptr
} };

void client::NetworkWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NetworkWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->socketError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->loginAccepted((*reinterpret_cast<std::add_pointer_t<quint64>>(_a[1]))); break;
        case 4: _t->joinedTable((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->actionReceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->stateReceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 7: _t->chatReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->rawMessageReceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 9: _t->connectToServer((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2]))); break;
        case 10: _t->disconnectFromServer(); break;
        case 11: _t->sendLogin((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->sendJoin(); break;
        case 13: _t->sendJoinWithBots(); break;
        case 14: _t->sendStartGame(); break;
        case 15: _t->sendLeave(); break;
        case 16: _t->sendDisconnect(); break;
        case 17: _t->sendCheck(); break;
        case 18: _t->sendCall(); break;
        case 19: _t->sendFold(); break;
        case 20: _t->sendRaise((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->onReadyRead(); break;
        case 22: _t->onConnected(); break;
        case 23: _t->onDisconnected(); break;
        case 24: _t->onError((*reinterpret_cast<std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 24:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)()>(_a, &NetworkWorker::connected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)()>(_a, &NetworkWorker::disconnected, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(const QString & )>(_a, &NetworkWorker::socketError, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(quint64 )>(_a, &NetworkWorker::loginAccepted, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(int )>(_a, &NetworkWorker::joinedTable, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(const QJsonObject & )>(_a, &NetworkWorker::actionReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(const QJsonObject & )>(_a, &NetworkWorker::stateReceived, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(const QString & )>(_a, &NetworkWorker::chatReceived, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkWorker::*)(const QJsonObject & )>(_a, &NetworkWorker::rawMessageReceived, 8))
            return;
    }
}

const QMetaObject *client::NetworkWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *client::NetworkWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client13NetworkWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int client::NetworkWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void client::NetworkWorker::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void client::NetworkWorker::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void client::NetworkWorker::socketError(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void client::NetworkWorker::loginAccepted(quint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void client::NetworkWorker::joinedTable(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void client::NetworkWorker::actionReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void client::NetworkWorker::stateReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void client::NetworkWorker::chatReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void client::NetworkWorker::rawMessageReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}
QT_WARNING_POP
