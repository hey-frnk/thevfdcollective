/****************************************************************************
** Meta object code from reading C++ file 'color_selector.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/color_selector.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_selector.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__ColorSelector_t {
    QByteArrayData data[20];
    char stringdata0[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__ColorSelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__ColorSelector_t qt_meta_stringdata_color_widgets__ColorSelector = {
    {
QT_MOC_LITERAL(0, 0, 28), // "color_widgets::ColorSelector"
QT_MOC_LITERAL(1, 29, 17), // "wheelFlagsChanged"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 24), // "ColorWheel::DisplayFlags"
QT_MOC_LITERAL(4, 73, 5), // "flags"
QT_MOC_LITERAL(5, 79, 17), // "updateModeChanged"
QT_MOC_LITERAL(6, 97, 10), // "UpdateMode"
QT_MOC_LITERAL(7, 108, 21), // "dialogModalityChanged"
QT_MOC_LITERAL(8, 130, 18), // "Qt::WindowModality"
QT_MOC_LITERAL(9, 149, 10), // "showDialog"
QT_MOC_LITERAL(10, 160, 13), // "setWheelFlags"
QT_MOC_LITERAL(11, 174, 13), // "accept_dialog"
QT_MOC_LITERAL(12, 188, 13), // "reject_dialog"
QT_MOC_LITERAL(13, 202, 16), // "update_old_color"
QT_MOC_LITERAL(14, 219, 1), // "c"
QT_MOC_LITERAL(15, 221, 10), // "updateMode"
QT_MOC_LITERAL(16, 232, 14), // "dialogModality"
QT_MOC_LITERAL(17, 247, 10), // "wheelFlags"
QT_MOC_LITERAL(18, 258, 7), // "Confirm"
QT_MOC_LITERAL(19, 266, 10) // "Continuous"

    },
    "color_widgets::ColorSelector\0"
    "wheelFlagsChanged\0\0ColorWheel::DisplayFlags\0"
    "flags\0updateModeChanged\0UpdateMode\0"
    "dialogModalityChanged\0Qt::WindowModality\0"
    "showDialog\0setWheelFlags\0accept_dialog\0"
    "reject_dialog\0update_old_color\0c\0"
    "updateMode\0dialogModality\0wheelFlags\0"
    "Confirm\0Continuous"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__ColorSelector[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       3,   72, // properties
       1,   84, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,
       7,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   63,    2, 0x0a /* Public */,
      10,    1,   64,    2, 0x0a /* Public */,
      11,    0,   67,    2, 0x08 /* Private */,
      12,    0,   68,    2, 0x08 /* Private */,
      13,    1,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,   14,

 // properties: name, type, flags
      15, 0x80000000 | 6, 0x0049510b,
      16, 0x80000000 | 8, 0x0049510b,
      17, 0x80000000 | 3, 0x0049510b,

 // properties: notify_signal_id
       1,
       2,
       0,

 // enums: name, alias, flags, count, data
       6,    6, 0x0,    2,   89,

 // enum data: key, value
      18, uint(color_widgets::ColorSelector::Confirm),
      19, uint(color_widgets::ColorSelector::Continuous),

       0        // eod
};

void color_widgets::ColorSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorSelector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wheelFlagsChanged((*reinterpret_cast< ColorWheel::DisplayFlags(*)>(_a[1]))); break;
        case 1: _t->updateModeChanged((*reinterpret_cast< UpdateMode(*)>(_a[1]))); break;
        case 2: _t->dialogModalityChanged((*reinterpret_cast< Qt::WindowModality(*)>(_a[1]))); break;
        case 3: _t->showDialog(); break;
        case 4: _t->setWheelFlags((*reinterpret_cast< ColorWheel::DisplayFlags(*)>(_a[1]))); break;
        case 5: _t->accept_dialog(); break;
        case 6: _t->reject_dialog(); break;
        case 7: _t->update_old_color((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorSelector::*)(ColorWheel::DisplayFlags );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorSelector::wheelFlagsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ColorSelector::*)(UpdateMode );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorSelector::updateModeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ColorSelector::*)(Qt::WindowModality );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorSelector::dialogModalityChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ColorSelector *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< UpdateMode*>(_v) = _t->updateMode(); break;
        case 1: *reinterpret_cast< Qt::WindowModality*>(_v) = _t->dialogModality(); break;
        case 2: *reinterpret_cast< ColorWheel::DisplayFlags*>(_v) = _t->wheelFlags(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ColorSelector *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUpdateMode(*reinterpret_cast< UpdateMode*>(_v)); break;
        case 1: _t->setDialogModality(*reinterpret_cast< Qt::WindowModality*>(_v)); break;
        case 2: _t->setWheelFlags(*reinterpret_cast< ColorWheel::DisplayFlags*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

static const QMetaObject::SuperData qt_meta_extradata_color_widgets__ColorSelector[] = {
    QMetaObject::SuperData::link<color_widgets::ColorWheel::staticMetaObject>(),
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject color_widgets::ColorSelector::staticMetaObject = { {
    QMetaObject::SuperData::link<ColorPreview::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__ColorSelector.data,
    qt_meta_data_color_widgets__ColorSelector,
    qt_static_metacall,
    qt_meta_extradata_color_widgets__ColorSelector,
    nullptr
} };


const QMetaObject *color_widgets::ColorSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::ColorSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__ColorSelector.stringdata0))
        return static_cast<void*>(this);
    return ColorPreview::qt_metacast(_clname);
}

int color_widgets::ColorSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ColorPreview::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void color_widgets::ColorSelector::wheelFlagsChanged(ColorWheel::DisplayFlags _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void color_widgets::ColorSelector::updateModeChanged(UpdateMode _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::ColorSelector::dialogModalityChanged(Qt::WindowModality _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
