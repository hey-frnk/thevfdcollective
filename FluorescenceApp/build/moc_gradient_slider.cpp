/****************************************************************************
** Meta object code from reading C++ file 'gradient_slider.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/gradient_slider.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gradient_slider.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__GradientSlider_t {
    QByteArrayData data[10];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__GradientSlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__GradientSlider_t qt_meta_stringdata_color_widgets__GradientSlider = {
    {
QT_MOC_LITERAL(0, 0, 29), // "color_widgets::GradientSlider"
QT_MOC_LITERAL(1, 30, 17), // "backgroundChanged"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 10), // "background"
QT_MOC_LITERAL(4, 60, 6), // "colors"
QT_MOC_LITERAL(5, 67, 14), // "QGradientStops"
QT_MOC_LITERAL(6, 82, 10), // "firstColor"
QT_MOC_LITERAL(7, 93, 9), // "lastColor"
QT_MOC_LITERAL(8, 103, 8), // "gradient"
QT_MOC_LITERAL(9, 112, 15) // "QLinearGradient"

    },
    "color_widgets::GradientSlider\0"
    "backgroundChanged\0\0background\0colors\0"
    "QGradientStops\0firstColor\0lastColor\0"
    "gradient\0QLinearGradient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__GradientSlider[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       5,   22, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QBrush,    2,

 // properties: name, type, flags
       3, QMetaType::QBrush, 0x00495103,
       4, 0x80000000 | 5, 0x0009410b,
       6, QMetaType::QColor, 0x00085103,
       7, QMetaType::QColor, 0x00085103,
       8, 0x80000000 | 9, 0x0009510b,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

void color_widgets::GradientSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GradientSlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backgroundChanged((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GradientSlider::*)(const QBrush & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GradientSlider::backgroundChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<GradientSlider *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QBrush*>(_v) = _t->background(); break;
        case 1: *reinterpret_cast< QGradientStops*>(_v) = _t->colors(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = _t->firstColor(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = _t->lastColor(); break;
        case 4: *reinterpret_cast< QLinearGradient*>(_v) = _t->gradient(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<GradientSlider *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBackground(*reinterpret_cast< QBrush*>(_v)); break;
        case 1: _t->setColors(*reinterpret_cast< QGradientStops*>(_v)); break;
        case 2: _t->setFirstColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: _t->setLastColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: _t->setGradient(*reinterpret_cast< QLinearGradient*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject color_widgets::GradientSlider::staticMetaObject = { {
    QMetaObject::SuperData::link<QSlider::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__GradientSlider.data,
    qt_meta_data_color_widgets__GradientSlider,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *color_widgets::GradientSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::GradientSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__GradientSlider.stringdata0))
        return static_cast<void*>(this);
    return QSlider::qt_metacast(_clname);
}

int color_widgets::GradientSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void color_widgets::GradientSlider::backgroundChanged(const QBrush & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
