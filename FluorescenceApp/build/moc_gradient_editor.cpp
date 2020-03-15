/****************************************************************************
** Meta object code from reading C++ file 'gradient_editor.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/gradient_editor.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gradient_editor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__GradientEditor_t {
    QByteArrayData data[21];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__GradientEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__GradientEditor_t qt_meta_stringdata_color_widgets__GradientEditor = {
    {
QT_MOC_LITERAL(0, 0, 29), // "color_widgets::GradientEditor"
QT_MOC_LITERAL(1, 30, 17), // "backgroundChanged"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 12), // "stopsChanged"
QT_MOC_LITERAL(4, 62, 14), // "QGradientStops"
QT_MOC_LITERAL(5, 77, 19), // "selectedStopChanged"
QT_MOC_LITERAL(6, 97, 14), // "setOrientation"
QT_MOC_LITERAL(7, 112, 15), // "Qt::Orientation"
QT_MOC_LITERAL(8, 128, 15), // "setSelectedStop"
QT_MOC_LITERAL(9, 144, 4), // "stop"
QT_MOC_LITERAL(10, 149, 16), // "setSelectedColor"
QT_MOC_LITERAL(11, 166, 5), // "color"
QT_MOC_LITERAL(12, 172, 7), // "addStop"
QT_MOC_LITERAL(13, 180, 10), // "removeStop"
QT_MOC_LITERAL(14, 191, 10), // "background"
QT_MOC_LITERAL(15, 202, 5), // "stops"
QT_MOC_LITERAL(16, 208, 8), // "gradient"
QT_MOC_LITERAL(17, 217, 15), // "QLinearGradient"
QT_MOC_LITERAL(18, 233, 11), // "orientation"
QT_MOC_LITERAL(19, 245, 12), // "selectedStop"
QT_MOC_LITERAL(20, 258, 13) // "selectedColor"

    },
    "color_widgets::GradientEditor\0"
    "backgroundChanged\0\0stopsChanged\0"
    "QGradientStops\0selectedStopChanged\0"
    "setOrientation\0Qt::Orientation\0"
    "setSelectedStop\0stop\0setSelectedColor\0"
    "color\0addStop\0removeStop\0background\0"
    "stops\0gradient\0QLinearGradient\0"
    "orientation\0selectedStop\0selectedColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__GradientEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       6,   74, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       3,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   63,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x0a /* Public */,
      10,    1,   69,    2, 0x0a /* Public */,
      12,    0,   72,    2, 0x0a /* Public */,
      13,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QBrush,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QColor,   11,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      14, QMetaType::QBrush, 0x00495103,
      15, 0x80000000 | 4, 0x0049510b,
      16, 0x80000000 | 17, 0x0009510b,
      18, 0x80000000 | 7, 0x0009510b,
      19, QMetaType::Int, 0x00495103,
      20, QMetaType::QColor, 0x00095103,

 // properties: notify_signal_id
       0,
       1,
       0,
       0,
       2,
       0,

       0        // eod
};

void color_widgets::GradientEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GradientEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backgroundChanged((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        case 1: _t->stopsChanged((*reinterpret_cast< const QGradientStops(*)>(_a[1]))); break;
        case 2: _t->selectedStopChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setOrientation((*reinterpret_cast< Qt::Orientation(*)>(_a[1]))); break;
        case 4: _t->setSelectedStop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setSelectedColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->addStop(); break;
        case 7: _t->removeStop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GradientEditor::*)(const QBrush & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GradientEditor::backgroundChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GradientEditor::*)(const QGradientStops & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GradientEditor::stopsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GradientEditor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GradientEditor::selectedStopChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<GradientEditor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QBrush*>(_v) = _t->background(); break;
        case 1: *reinterpret_cast< QGradientStops*>(_v) = _t->stops(); break;
        case 2: *reinterpret_cast< QLinearGradient*>(_v) = _t->gradient(); break;
        case 3: *reinterpret_cast< Qt::Orientation*>(_v) = _t->orientation(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->selectedStop(); break;
        case 5: *reinterpret_cast< QColor*>(_v) = _t->selectedColor(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<GradientEditor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBackground(*reinterpret_cast< QBrush*>(_v)); break;
        case 1: _t->setStops(*reinterpret_cast< QGradientStops*>(_v)); break;
        case 2: _t->setGradient(*reinterpret_cast< QLinearGradient*>(_v)); break;
        case 3: _t->setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 4: _t->setSelectedStop(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setSelectedColor(*reinterpret_cast< QColor*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject color_widgets::GradientEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__GradientEditor.data,
    qt_meta_data_color_widgets__GradientEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *color_widgets::GradientEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::GradientEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__GradientEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int color_widgets::GradientEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void color_widgets::GradientEditor::backgroundChanged(const QBrush & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void color_widgets::GradientEditor::stopsChanged(const QGradientStops & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::GradientEditor::selectedStopChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
