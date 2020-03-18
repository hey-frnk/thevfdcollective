/****************************************************************************
** Meta object code from reading C++ file 'color_preview.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/color_preview.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_preview.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__ColorPreview_t {
    QByteArrayData data[19];
    char stringdata0[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__ColorPreview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__ColorPreview_t qt_meta_stringdata_color_widgets__ColorPreview = {
    {
QT_MOC_LITERAL(0, 0, 27), // "color_widgets::ColorPreview"
QT_MOC_LITERAL(1, 28, 7), // "clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 12), // "colorChanged"
QT_MOC_LITERAL(4, 50, 22), // "comparisonColorChanged"
QT_MOC_LITERAL(5, 73, 18), // "displayModeChanged"
QT_MOC_LITERAL(6, 92, 11), // "DisplayMode"
QT_MOC_LITERAL(7, 104, 17), // "backgroundChanged"
QT_MOC_LITERAL(8, 122, 8), // "setColor"
QT_MOC_LITERAL(9, 131, 1), // "c"
QT_MOC_LITERAL(10, 133, 18), // "setComparisonColor"
QT_MOC_LITERAL(11, 152, 5), // "color"
QT_MOC_LITERAL(12, 158, 15), // "comparisonColor"
QT_MOC_LITERAL(13, 174, 12), // "display_mode"
QT_MOC_LITERAL(14, 187, 10), // "background"
QT_MOC_LITERAL(15, 198, 7), // "NoAlpha"
QT_MOC_LITERAL(16, 206, 8), // "AllAlpha"
QT_MOC_LITERAL(17, 215, 10), // "SplitAlpha"
QT_MOC_LITERAL(18, 226, 10) // "SplitColor"

    },
    "color_widgets::ColorPreview\0clicked\0"
    "\0colorChanged\0comparisonColorChanged\0"
    "displayModeChanged\0DisplayMode\0"
    "backgroundChanged\0setColor\0c\0"
    "setComparisonColor\0color\0comparisonColor\0"
    "display_mode\0background\0NoAlpha\0"
    "AllAlpha\0SplitAlpha\0SplitColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__ColorPreview[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       4,   68, // properties
       1,   84, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,
       4,    1,   53,    2, 0x06 /* Public */,
       5,    1,   56,    2, 0x06 /* Public */,
       7,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   62,    2, 0x0a /* Public */,
      10,    1,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::QBrush,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    9,
    QMetaType::Void, QMetaType::QColor,    9,

 // properties: name, type, flags
      11, QMetaType::QColor, 0x00495103,
      12, QMetaType::QColor, 0x00495103,
      13, 0x80000000 | 6, 0x0049500b,
      14, QMetaType::QBrush, 0x00495103,

 // properties: notify_signal_id
       1,
       2,
       3,
       4,

 // enums: name, alias, flags, count, data
       6,    6, 0x0,    4,   89,

 // enum data: key, value
      15, uint(color_widgets::ColorPreview::NoAlpha),
      16, uint(color_widgets::ColorPreview::AllAlpha),
      17, uint(color_widgets::ColorPreview::SplitAlpha),
      18, uint(color_widgets::ColorPreview::SplitColor),

       0        // eod
};

void color_widgets::ColorPreview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorPreview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->colorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->comparisonColorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: _t->displayModeChanged((*reinterpret_cast< DisplayMode(*)>(_a[1]))); break;
        case 4: _t->backgroundChanged((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        case 5: _t->setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->setComparisonColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorPreview::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPreview::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ColorPreview::*)(QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPreview::colorChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ColorPreview::*)(QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPreview::comparisonColorChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ColorPreview::*)(DisplayMode );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPreview::displayModeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ColorPreview::*)(const QBrush & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPreview::backgroundChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ColorPreview *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = _t->comparisonColor(); break;
        case 2: *reinterpret_cast< DisplayMode*>(_v) = _t->displayMode(); break;
        case 3: *reinterpret_cast< QBrush*>(_v) = _t->background(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ColorPreview *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: _t->setComparisonColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: _t->setDisplayMode(*reinterpret_cast< DisplayMode*>(_v)); break;
        case 3: _t->setBackground(*reinterpret_cast< QBrush*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject color_widgets::ColorPreview::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__ColorPreview.data,
    qt_meta_data_color_widgets__ColorPreview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *color_widgets::ColorPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::ColorPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__ColorPreview.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int color_widgets::ColorPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void color_widgets::ColorPreview::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void color_widgets::ColorPreview::colorChanged(QColor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::ColorPreview::comparisonColorChanged(QColor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void color_widgets::ColorPreview::displayModeChanged(DisplayMode _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void color_widgets::ColorPreview::backgroundChanged(const QBrush & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
