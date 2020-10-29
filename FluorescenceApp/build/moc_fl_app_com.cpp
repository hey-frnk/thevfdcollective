/****************************************************************************
** Meta object code from reading C++ file 'fl_app_com.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../fl_app_com.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fl_app_com.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_fl_app_com_t {
    QByteArrayData data[19];
    char stringdata0[409];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fl_app_com_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fl_app_com_t qt_meta_stringdata_fl_app_com = {
    {
QT_MOC_LITERAL(0, 0, 10), // "fl_app_com"
QT_MOC_LITERAL(1, 11, 17), // "app_com_connected"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "bt_incoming_data"
QT_MOC_LITERAL(4, 47, 17), // "bt_status_changed"
QT_MOC_LITERAL(5, 65, 7), // "message"
QT_MOC_LITERAL(6, 73, 21), // "bt_discovery_finished"
QT_MOC_LITERAL(7, 95, 27), // "bt_connect_device_connected"
QT_MOC_LITERAL(8, 123, 23), // "bt_connect_device_error"
QT_MOC_LITERAL(9, 147, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(10, 175, 30), // "bt_connect_device_disconnected"
QT_MOC_LITERAL(11, 206, 23), // "bt_connect_services_add"
QT_MOC_LITERAL(12, 230, 14), // "QBluetoothUuid"
QT_MOC_LITERAL(13, 245, 11), // "serviceUuid"
QT_MOC_LITERAL(14, 257, 33), // "bt_connect_services_scan_comp..."
QT_MOC_LITERAL(15, 291, 38), // "bt_connect_services_details_d..."
QT_MOC_LITERAL(16, 330, 31), // "QLowEnergyService::ServiceState"
QT_MOC_LITERAL(17, 362, 21), // "characteristicChanged"
QT_MOC_LITERAL(18, 384, 24) // "QLowEnergyCharacteristic"

    },
    "fl_app_com\0app_com_connected\0\0"
    "bt_incoming_data\0bt_status_changed\0"
    "message\0bt_discovery_finished\0"
    "bt_connect_device_connected\0"
    "bt_connect_device_error\0"
    "QLowEnergyController::Error\0"
    "bt_connect_device_disconnected\0"
    "bt_connect_services_add\0QBluetoothUuid\0"
    "serviceUuid\0bt_connect_services_scan_complete\0"
    "bt_connect_services_details_discovered\0"
    "QLowEnergyService::ServiceState\0"
    "characteristicChanged\0QLowEnergyCharacteristic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fl_app_com[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    1,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   74,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    1,   76,    2, 0x08 /* Private */,
      10,    0,   79,    2, 0x08 /* Private */,
      11,    1,   80,    2, 0x08 /* Private */,
      14,    0,   83,    2, 0x08 /* Private */,
      15,    1,   84,    2, 0x08 /* Private */,
      17,    2,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,    2,
    QMetaType::Void, 0x80000000 | 18, QMetaType::QByteArray,    2,    2,

       0        // eod
};

void fl_app_com::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fl_app_com *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->app_com_connected(); break;
        case 1: _t->bt_incoming_data(); break;
        case 2: _t->bt_status_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->bt_discovery_finished(); break;
        case 4: _t->bt_connect_device_connected(); break;
        case 5: _t->bt_connect_device_error((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 6: _t->bt_connect_device_disconnected(); break;
        case 7: _t->bt_connect_services_add((*reinterpret_cast< const QBluetoothUuid(*)>(_a[1]))); break;
        case 8: _t->bt_connect_services_scan_complete(); break;
        case 9: _t->bt_connect_services_details_discovered((*reinterpret_cast< QLowEnergyService::ServiceState(*)>(_a[1]))); break;
        case 10: _t->characteristicChanged((*reinterpret_cast< QLowEnergyCharacteristic(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController::Error >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothUuid >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyService::ServiceState >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyCharacteristic >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (fl_app_com::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fl_app_com::app_com_connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (fl_app_com::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fl_app_com::bt_incoming_data)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (fl_app_com::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fl_app_com::bt_status_changed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fl_app_com::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_fl_app_com.data,
    qt_meta_data_fl_app_com,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fl_app_com::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fl_app_com::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fl_app_com.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int fl_app_com::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void fl_app_com::app_com_connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void fl_app_com::bt_incoming_data()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void fl_app_com::bt_status_changed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
