/****************************************************************************
** Meta object code from reading C++ file 'color_palette_model.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtColorWidgets/color_palette_model.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_palette_model.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_color_widgets__ColorPaletteModel_t {
    QByteArrayData data[14];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_color_widgets__ColorPaletteModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_color_widgets__ColorPaletteModel_t qt_meta_stringdata_color_widgets__ColorPaletteModel = {
    {
QT_MOC_LITERAL(0, 0, 32), // "color_widgets::ColorPaletteModel"
QT_MOC_LITERAL(1, 33, 15), // "savePathChanged"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 8), // "savePath"
QT_MOC_LITERAL(4, 59, 18), // "searchPathsChanged"
QT_MOC_LITERAL(5, 78, 11), // "searchPaths"
QT_MOC_LITERAL(6, 90, 15), // "iconSizeChanged"
QT_MOC_LITERAL(7, 106, 8), // "iconSize"
QT_MOC_LITERAL(8, 115, 11), // "setSavePath"
QT_MOC_LITERAL(9, 127, 14), // "setSearchPaths"
QT_MOC_LITERAL(10, 142, 13), // "addSearchPath"
QT_MOC_LITERAL(11, 156, 4), // "path"
QT_MOC_LITERAL(12, 161, 11), // "setIconSize"
QT_MOC_LITERAL(13, 173, 4) // "load"

    },
    "color_widgets::ColorPaletteModel\0"
    "savePathChanged\0\0savePath\0searchPathsChanged\0"
    "searchPaths\0iconSizeChanged\0iconSize\0"
    "setSavePath\0setSearchPaths\0addSearchPath\0"
    "path\0setIconSize\0load"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_color_widgets__ColorPaletteModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       3,   76, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   63,    2, 0x0a /* Public */,
       9,    1,   66,    2, 0x0a /* Public */,
      10,    1,   69,    2, 0x0a /* Public */,
      12,    1,   72,    2, 0x0a /* Public */,
      13,    0,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    5,
    QMetaType::Void, QMetaType::QSize,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    5,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QSize,    7,
    QMetaType::Void,

 // properties: name, type, flags
       5, QMetaType::QStringList, 0x00495103,
       3, QMetaType::QString, 0x00495103,
       7, QMetaType::QSize, 0x00495103,

 // properties: notify_signal_id
       1,
       0,
       2,

       0        // eod
};

void color_widgets::ColorPaletteModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorPaletteModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->savePathChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->searchPathsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->iconSizeChanged((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        case 3: _t->setSavePath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->setSearchPaths((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->addSearchPath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->setIconSize((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        case 7: _t->load(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorPaletteModel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPaletteModel::savePathChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ColorPaletteModel::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPaletteModel::searchPathsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ColorPaletteModel::*)(const QSize & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ColorPaletteModel::iconSizeChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ColorPaletteModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->searchPaths(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->savePath(); break;
        case 2: *reinterpret_cast< QSize*>(_v) = _t->iconSize(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ColorPaletteModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSearchPaths(*reinterpret_cast< QStringList*>(_v)); break;
        case 1: _t->setSavePath(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setIconSize(*reinterpret_cast< QSize*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject color_widgets::ColorPaletteModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_meta_stringdata_color_widgets__ColorPaletteModel.data,
    qt_meta_data_color_widgets__ColorPaletteModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *color_widgets::ColorPaletteModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *color_widgets::ColorPaletteModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_color_widgets__ColorPaletteModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int color_widgets::ColorPaletteModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
void color_widgets::ColorPaletteModel::savePathChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void color_widgets::ColorPaletteModel::searchPathsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void color_widgets::ColorPaletteModel::iconSizeChanged(const QSize & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
