/****************************************************************************
** Meta object code from reading C++ file 'fwupdate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../fwupdate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fwupdate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FWUpdate_t {
    QByteArrayData data[15];
    char stringdata0[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FWUpdate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FWUpdate_t qt_meta_stringdata_FWUpdate = {
    {
QT_MOC_LITERAL(0, 0, 8), // "FWUpdate"
QT_MOC_LITERAL(1, 9, 23), // "readyReadStandardOutput"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 22), // "readyReadStandardError"
QT_MOC_LITERAL(4, 57, 23), // "on_welcome_next_clicked"
QT_MOC_LITERAL(5, 81, 22), // "on_updater_run_clicked"
QT_MOC_LITERAL(6, 104, 29), // "on_updater_agree_stateChanged"
QT_MOC_LITERAL(7, 134, 4), // "arg1"
QT_MOC_LITERAL(8, 139, 25), // "on_updater_update_clicked"
QT_MOC_LITERAL(9, 165, 7), // "ReadOut"
QT_MOC_LITERAL(10, 173, 7), // "ReadErr"
QT_MOC_LITERAL(11, 181, 14), // "UpdateFinished"
QT_MOC_LITERAL(12, 196, 8), // "exitCode"
QT_MOC_LITERAL(13, 205, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(14, 226, 10) // "exitStatus"

    },
    "FWUpdate\0readyReadStandardOutput\0\0"
    "readyReadStandardError\0on_welcome_next_clicked\0"
    "on_updater_run_clicked\0"
    "on_updater_agree_stateChanged\0arg1\0"
    "on_updater_update_clicked\0ReadOut\0"
    "ReadErr\0UpdateFinished\0exitCode\0"
    "QProcess::ExitStatus\0exitStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FWUpdate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    1,   63,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    2,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 13,   12,   14,

       0        // eod
};

void FWUpdate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FWUpdate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readyReadStandardOutput(); break;
        case 1: _t->readyReadStandardError(); break;
        case 2: _t->on_welcome_next_clicked(); break;
        case 3: _t->on_updater_run_clicked(); break;
        case 4: _t->on_updater_agree_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_updater_update_clicked(); break;
        case 6: _t->ReadOut(); break;
        case 7: _t->ReadErr(); break;
        case 8: _t->UpdateFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FWUpdate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FWUpdate::readyReadStandardOutput)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FWUpdate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FWUpdate::readyReadStandardError)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FWUpdate::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_FWUpdate.data,
    qt_meta_data_FWUpdate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FWUpdate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FWUpdate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FWUpdate.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int FWUpdate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FWUpdate::readyReadStandardOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FWUpdate::readyReadStandardError()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
