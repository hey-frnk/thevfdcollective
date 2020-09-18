/****************************************************************************
** Meta object code from reading C++ file 'fl_app_com.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
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
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_fl_app_com_t {
    QByteArrayData data[16];
    char stringdata0[366];
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
QT_MOC_LITERAL(3, 30, 21), // "bt_discovery_finished"
QT_MOC_LITERAL(4, 52, 27), // "bt_connect_device_connected"
QT_MOC_LITERAL(5, 80, 23), // "bt_connect_device_error"
QT_MOC_LITERAL(6, 104, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(7, 132, 30), // "bt_connect_device_disconnected"
QT_MOC_LITERAL(8, 163, 23), // "bt_connect_services_add"
QT_MOC_LITERAL(9, 187, 14), // "QBluetoothUuid"
QT_MOC_LITERAL(10, 202, 11), // "serviceUuid"
QT_MOC_LITERAL(11, 214, 33), // "bt_connect_services_scan_comp..."
QT_MOC_LITERAL(12, 248, 38), // "bt_connect_services_details_d..."
QT_MOC_LITERAL(13, 287, 31), // "QLowEnergyService::ServiceState"
QT_MOC_LITERAL(14, 319, 21), // "characteristicChanged"
QT_MOC_LITERAL(15, 341, 24) // "QLowEnergyCharacteristic"

    },
    "fl_app_com\0app_com_connected\0\0"
    "bt_discovery_finished\0bt_connect_device_connected\0"
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    1,   62,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    1,   66,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,
      12,    1,   70,    2, 0x08 /* Private */,
      14,    2,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 15, QMetaType::QByteArray,    2,    2,

       0        // eod
};

void fl_app_com::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fl_app_com *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->app_com_connected(); break;
        case 1: _t->bt_discovery_finished(); break;
        case 2: _t->bt_connect_device_connected(); break;
        case 3: _t->bt_connect_device_error((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 4: _t->bt_connect_device_disconnected(); break;
        case 5: _t->bt_connect_services_add((*reinterpret_cast< const QBluetoothUuid(*)>(_a[1]))); break;
        case 6: _t->bt_connect_services_scan_complete(); break;
        case 7: _t->bt_connect_services_details_discovered((*reinterpret_cast< QLowEnergyService::ServiceState(*)>(_a[1]))); break;
        case 8: _t->characteristicChanged((*reinterpret_cast< QLowEnergyCharacteristic(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController::Error >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothUuid >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyService::ServiceState >(); break;
            }
            break;
        case 8:
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void fl_app_com::app_com_connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
