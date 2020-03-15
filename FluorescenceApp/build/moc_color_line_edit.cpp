/****************************************************************************
** Meta object code from reading C++ file 'color_line_edit.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/color_line_edit.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_line_edit.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__ColorLineEdit_t {
    QByteArrayData data[13];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__ColorLineEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__ColorLineEdit_t qt_meta_stringdata_color_widgets__ColorLineEdit = {
    {
QT_MOC_LITERAL(0, 0, 28), // "color_widgets::ColorLineEdit"
QT_MOC_LITERAL(1, 29, 12), // "colorChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 5), // "color"
QT_MOC_LITERAL(4, 49, 11), // "colorEdited"
QT_MOC_LITERAL(5, 61, 20), // "colorEditingFinished"
QT_MOC_LITERAL(6, 82, 16), // "showAlphaChanged"
QT_MOC_LITERAL(7, 99, 9), // "showAlpha"
QT_MOC_LITERAL(8, 109, 19), // "previewColorChanged"
QT_MOC_LITERAL(9, 129, 12), // "previewColor"
QT_MOC_LITERAL(10, 142, 8), // "setColor"
QT_MOC_LITERAL(11, 151, 12), // "setShowAlpha"
QT_MOC_LITERAL(12, 164, 15) // "setPreviewColor"

    },
    "color_widgets::ColorLineEdit\0colorChanged\0"
    "\0color\0colorEdited\0colorEditingFinished\0"
    "showAlphaChanged\0showAlpha\0"
    "previewColorChanged\0previewColor\0"
    "setColor\0setShowAlpha\0setPreviewColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__ColorLineEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       3,   78, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,
       8,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   69,    2, 0x0a /* Public */,
      11,    1,   72,    2, 0x0a /* Public */,
      12,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    9,

 // properties: name, type, flags
       3, QMetaType::QColor, 0x00495103,
       7, QMetaType::Bool, 0x00495103,
       9, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,
       3,
       4,

       0        // eod
};

void color_widgets::ColorLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorLineEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 1: _t->colorEdited((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 2: _t->colorEditingFinished((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 3: _t->showAlphaChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->previewColorChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->setShowAlpha((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setPreviewColor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorLineEdit::*)(const QColor & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorLineEdit::colorChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ColorLineEdit::*)(const QColor & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorLineEdit::colorEdited)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ColorLineEdit::*)(const QColor & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorLineEdit::colorEditingFinished)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ColorLineEdit::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorLineEdit::showAlphaChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ColorLineEdit::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorLineEdit::previewColorChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ColorLineEdit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->showAlpha(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->previewColor(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ColorLineEdit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: _t->setShowAlpha(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setPreviewColor(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject color_widgets::ColorLineEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QLineEdit::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__ColorLineEdit.data,
    qt_meta_data_color_widgets__ColorLineEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *color_widgets::ColorLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::ColorLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__ColorLineEdit.stringdata0))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int color_widgets::ColorLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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
void color_widgets::ColorLineEdit::colorChanged(const QColor & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void color_widgets::ColorLineEdit::colorEdited(const QColor & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::ColorLineEdit::colorEditingFinished(const QColor & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void color_widgets::ColorLineEdit::showAlphaChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void color_widgets::ColorLineEdit::previewColorChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
