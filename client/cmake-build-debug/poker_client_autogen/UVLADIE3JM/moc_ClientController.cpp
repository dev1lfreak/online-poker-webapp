/****************************************************************************
** Meta object code from reading C++ file 'ClientController.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ClientController.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClientController.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6client16ClientControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto client::ClientController::qt_create_metaobjectdata<qt_meta_tag_ZN6client16ClientControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "client::ClientController",
        "statusChanged",
        "",
        "status",
        "loginCompleted",
        "playerId",
        "tableJoined",
        "tableId",
        "tableStateUpdated",
        "QJsonObject",
        "state",
        "waitingRoomUpdated",
        "tableActionReceived",
        "action",
        "chatMessageReceived",
        "message",
        "gameStarted",
        "gameEnded",
        "requestConnect",
        "host",
        "port",
        "requestDisconnect",
        "requestLogin",
        "nickname",
        "requestFindGame",
        "requestFindGameWithBots",
        "requestStartGame",
        "requestLeaveTable",
        "requestSendDisconnect",
        "requestCheck",
        "requestCall",
        "requestFold",
        "requestRaise",
        "amount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'statusChanged'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'loginCompleted'
        QtMocHelpers::SignalData<void(quint64)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::ULongLong, 5 },
        }}),
        // Signal 'tableJoined'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'tableStateUpdated'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Signal 'waitingRoomUpdated'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Signal 'tableActionReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 13 },
        }}),
        // Signal 'chatMessageReceived'
        QtMocHelpers::SignalData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Signal 'gameStarted'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameEnded'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestConnect'
        QtMocHelpers::SignalData<void(const QString &, quint16)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 }, { QMetaType::UShort, 20 },
        }}),
        // Signal 'requestDisconnect'
        QtMocHelpers::SignalData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestLogin'
        QtMocHelpers::SignalData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Signal 'requestFindGame'
        QtMocHelpers::SignalData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestFindGameWithBots'
        QtMocHelpers::SignalData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestStartGame'
        QtMocHelpers::SignalData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestLeaveTable'
        QtMocHelpers::SignalData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestSendDisconnect'
        QtMocHelpers::SignalData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestCheck'
        QtMocHelpers::SignalData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestCall'
        QtMocHelpers::SignalData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestFold'
        QtMocHelpers::SignalData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestRaise'
        QtMocHelpers::SignalData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ClientController, qt_meta_tag_ZN6client16ClientControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject client::ClientController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client16ClientControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client16ClientControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6client16ClientControllerE_t>.metaTypes,
    nullptr
} };

void client::ClientController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ClientController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->loginCompleted((*reinterpret_cast<std::add_pointer_t<quint64>>(_a[1]))); break;
        case 2: _t->tableJoined((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->tableStateUpdated((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 4: _t->waitingRoomUpdated((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 5: _t->tableActionReceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->chatMessageReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->gameStarted(); break;
        case 8: _t->gameEnded(); break;
        case 9: _t->requestConnect((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2]))); break;
        case 10: _t->requestDisconnect(); break;
        case 11: _t->requestLogin((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->requestFindGame(); break;
        case 13: _t->requestFindGameWithBots(); break;
        case 14: _t->requestStartGame(); break;
        case 15: _t->requestLeaveTable(); break;
        case 16: _t->requestSendDisconnect(); break;
        case 17: _t->requestCheck(); break;
        case 18: _t->requestCall(); break;
        case 19: _t->requestFold(); break;
        case 20: _t->requestRaise((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QString & )>(_a, &ClientController::statusChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(quint64 )>(_a, &ClientController::loginCompleted, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(int )>(_a, &ClientController::tableJoined, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QJsonObject & )>(_a, &ClientController::tableStateUpdated, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QJsonObject & )>(_a, &ClientController::waitingRoomUpdated, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QJsonObject & )>(_a, &ClientController::tableActionReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QString & )>(_a, &ClientController::chatMessageReceived, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::gameStarted, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::gameEnded, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QString & , quint16 )>(_a, &ClientController::requestConnect, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestDisconnect, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(const QString & )>(_a, &ClientController::requestLogin, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestFindGame, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestFindGameWithBots, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestStartGame, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestLeaveTable, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestSendDisconnect, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestCheck, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestCall, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)()>(_a, &ClientController::requestFold, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClientController::*)(int )>(_a, &ClientController::requestRaise, 20))
            return;
    }
}

const QMetaObject *client::ClientController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *client::ClientController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6client16ClientControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int client::ClientController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void client::ClientController::statusChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void client::ClientController::loginCompleted(quint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void client::ClientController::tableJoined(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void client::ClientController::tableStateUpdated(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void client::ClientController::waitingRoomUpdated(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void client::ClientController::tableActionReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void client::ClientController::chatMessageReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void client::ClientController::gameStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void client::ClientController::gameEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void client::ClientController::requestConnect(const QString & _t1, quint16 _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void client::ClientController::requestDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void client::ClientController::requestLogin(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void client::ClientController::requestFindGame()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void client::ClientController::requestFindGameWithBots()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void client::ClientController::requestStartGame()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void client::ClientController::requestLeaveTable()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void client::ClientController::requestSendDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void client::ClientController::requestCheck()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void client::ClientController::requestCall()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void client::ClientController::requestFold()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void client::ClientController::requestRaise(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1);
}
QT_WARNING_POP
