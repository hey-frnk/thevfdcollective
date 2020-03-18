/****************************************************************************
** Meta object code from reading C++ file 'color_dialog.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/color_dialog.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_dialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__ColorDialog_t {
    QByteArrayData data[30];
    char stringdata0[395];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__ColorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__ColorDialog_t qt_meta_stringdata_color_widgets__ColorDialog = {
    {
QT_MOC_LITERAL(0, 0, 26), // "color_widgets::ColorDialog"
QT_MOC_LITERAL(1, 27, 12), // "colorChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 13), // "colorSelected"
QT_MOC_LITERAL(4, 55, 17), // "wheelFlagsChanged"
QT_MOC_LITERAL(5, 73, 24), // "ColorWheel::DisplayFlags"
QT_MOC_LITERAL(6, 98, 5), // "flags"
QT_MOC_LITERAL(7, 104, 19), // "alphaEnabledChanged"
QT_MOC_LITERAL(8, 124, 12), // "alphaEnabled"
QT_MOC_LITERAL(9, 137, 8), // "setColor"
QT_MOC_LITERAL(10, 146, 1), // "c"
QT_MOC_LITERAL(11, 148, 9), // "showColor"
QT_MOC_LITERAL(12, 158, 8), // "oldcolor"
QT_MOC_LITERAL(13, 167, 13), // "setWheelFlags"
QT_MOC_LITERAL(14, 181, 15), // "setAlphaEnabled"
QT_MOC_LITERAL(15, 197, 1), // "a"
QT_MOC_LITERAL(16, 199, 16), // "setColorInternal"
QT_MOC_LITERAL(17, 216, 5), // "color"
QT_MOC_LITERAL(18, 222, 7), // "set_hsv"
QT_MOC_LITERAL(19, 230, 7), // "set_rgb"
QT_MOC_LITERAL(20, 238, 9), // "set_alpha"
QT_MOC_LITERAL(21, 248, 24), // "on_edit_hex_colorChanged"
QT_MOC_LITERAL(22, 273, 32), // "on_edit_hex_colorEditingFinished"
QT_MOC_LITERAL(23, 306, 20), // "on_buttonBox_clicked"
QT_MOC_LITERAL(24, 327, 16), // "QAbstractButton*"
QT_MOC_LITERAL(25, 344, 10), // "wheelFlags"
QT_MOC_LITERAL(26, 355, 10), // "ButtonMode"
QT_MOC_LITERAL(27, 366, 8), // "OkCancel"
QT_MOC_LITERAL(28, 375, 13), // "OkApplyCancel"
QT_MOC_LITERAL(29, 389, 5) // "Close"

    },
    "color_widgets::ColorDialog\0colorChanged\0"
    "\0colorSelected\0wheelFlagsChanged\0"
    "ColorWheel::DisplayFlags\0flags\0"
    "alphaEnabledChanged\0alphaEnabled\0"
    "setColor\0c\0showColor\0oldcolor\0"
    "setWheelFlags\0setAlphaEnabled\0a\0"
    "setColorInternal\0color\0set_hsv\0set_rgb\0"
    "set_alpha\0on_edit_hex_colorChanged\0"
    "on_edit_hex_colorEditingFinished\0"
    "on_buttonBox_clicked\0QAbstractButton*\0"
    "wheelFlags\0ButtonMode\0OkCancel\0"
    "OkApplyCancel\0Close"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__ColorDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       3,  128, // properties
       1,  140, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       3,    1,   92,    2, 0x06 /* Public */,
       4,    1,   95,    2, 0x06 /* Public */,
       7,    1,   98,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  101,    2, 0x0a /* Public */,
      11,    1,  104,    2, 0x0a /* Public */,
      13,    1,  107,    2, 0x0a /* Public */,
      14,    1,  110,    2, 0x0a /* Public */,
      16,    1,  113,    2, 0x08 /* Private */,
      18,    0,  116,    2, 0x08 /* Private */,
      19,    0,  117,    2, 0x08 /* Private */,
      20,    0,  118,    2, 0x08 /* Private */,
      21,    1,  119,    2, 0x08 /* Private */,
      22,    1,  122,    2, 0x08 /* Private */,
      23,    1,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,   10,
    QMetaType::Void, QMetaType::QColor,   12,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QColor,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,   17,
    QMetaType::Void, QMetaType::QColor,   17,
    QMetaType::Void, 0x80000000 | 24,    2,

 // properties: name, type, flags
      17, QMetaType::QColor, 0x00495103,
      25, 0x80000000 | 5, 0x0049510b,
       8, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,
       2,
       3,

 // enums: name, alias, flags, count, data
      26,   26, 0x0,    3,  145,

 // enum data: key, value
      27, uint(color_widgets::ColorDialog::OkCancel),
      28, uint(color_widgets::ColorDialog::OkApplyCancel),
      29, uint(color_widgets::ColorDialog::Close),

       0        // eod
};

void color_widgets::ColorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->colorSelected((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->wheelFlagsChanged((*reinterpret_cast< ColorWheel::DisplayFlags(*)>(_a[1]))); break;
        case 3: _t->alphaEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->showColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->setWheelFlags((*reinterpret_cast< ColorWheel::DisplayFlags(*)>(_a[1]))); break;
        case 7: _t->setAlphaEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setColorInternal((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 9: _t->set_hsv(); break;
        case 10: _t->set_rgb(); break;
        case 11: _t->set_alpha(); break;
        case 12: _t->on_edit_hex_colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 13: _t->on_edit_hex_colorEditingFinished((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 14: _t->on_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorDialog::*)(QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorDialog::colorChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ColorDialog::*)(QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorDialog::colorSelected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ColorDialog::*)(ColorWheel::DisplayFlags );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorDialog::wheelFlagsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ColorDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorDialog::alphaEnabledChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ColorDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< ColorWheel::DisplayFlags*>(_v) = _t->wheelFlags(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->alphaEnabled(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ColorDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: _t->setWheelFlags(*reinterpret_cast< ColorWheel::DisplayFlags*>(_v)); break;
        case 2: _t->setAlphaEnabled(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

static const QMetaObject::SuperData qt_meta_extradata_color_widgets__ColorDialog[] = {
    QMetaObject::SuperData::link<color_widgets::ColorWheel::staticMetaObject>(),
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject color_widgets::ColorDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__ColorDialog.data,
    qt_meta_data_color_widgets__ColorDialog,
    qt_static_metacall,
    qt_meta_extradata_color_widgets__ColorDialog,
    nullptr
} };


const QMetaObject *color_widgets::ColorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::ColorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__ColorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int color_widgets::ColorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
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
void color_widgets::ColorDialog::colorChanged(QColor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void color_widgets::ColorDialog::colorSelected(QColor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::ColorDialog::wheelFlagsChanged(ColorWheel::DisplayFlags _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void color_widgets::ColorDialog::alphaEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
