/****************************************************************************
** Meta object code from reading C++ file 'fwupdatestm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../fwupdatestm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fwupdatestm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FWUpdateSTM_t {
    QByteArrayData data[7];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FWUpdateSTM_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FWUpdateSTM_t qt_meta_stringdata_FWUpdateSTM = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FWUpdateSTM"
QT_MOC_LITERAL(1, 12, 23), // "on_welcome_next_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 22), // "on_updater_run_clicked"
QT_MOC_LITERAL(4, 60, 29), // "on_updater_agree_stateChanged"
QT_MOC_LITERAL(5, 90, 4), // "arg1"
QT_MOC_LITERAL(6, 95, 25) // "on_updater_update_clicked"

    },
    "FWUpdateSTM\0on_welcome_next_clicked\0"
    "\0on_updater_run_clicked\0"
    "on_updater_agree_stateChanged\0arg1\0"
    "on_updater_update_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FWUpdateSTM[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    1,   36,    2, 0x08 /* Private */,
       6,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

       0        // eod
};

void FWUpdateSTM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FWUpdateSTM *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_welcome_next_clicked(); break;
        case 1: _t->on_updater_run_clicked(); break;
        case 2: _t->on_updater_agree_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_updater_update_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FWUpdateSTM::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_FWUpdateSTM.data,
    qt_meta_data_FWUpdateSTM,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FWUpdateSTM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FWUpdateSTM::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FWUpdateSTM.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int FWUpdateSTM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE