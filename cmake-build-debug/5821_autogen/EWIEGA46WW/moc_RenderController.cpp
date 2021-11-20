/****************************************************************************
** Meta object code from reading C++ file 'RenderController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../RenderController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderController_t {
    QByteArrayData data[13];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderController_t qt_meta_stringdata_RenderController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "RenderController"
QT_MOC_LITERAL(1, 17, 21), // "objectRotationChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 11), // "zoomChanged"
QT_MOC_LITERAL(4, 52, 5), // "value"
QT_MOC_LITERAL(5, 58, 17), // "xTranslateChanged"
QT_MOC_LITERAL(6, 76, 17), // "yTranslateChanged"
QT_MOC_LITERAL(7, 94, 15), // "BeginScaledDrag"
QT_MOC_LITERAL(8, 110, 11), // "whichButton"
QT_MOC_LITERAL(9, 122, 1), // "x"
QT_MOC_LITERAL(10, 124, 1), // "y"
QT_MOC_LITERAL(11, 126, 18), // "ContinueScaledDrag"
QT_MOC_LITERAL(12, 145, 13) // "EndScaledDrag"

    },
    "RenderController\0objectRotationChanged\0"
    "\0zoomChanged\0value\0xTranslateChanged\0"
    "yTranslateChanged\0BeginScaledDrag\0"
    "whichButton\0x\0y\0ContinueScaledDrag\0"
    "EndScaledDrag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    1,   50,    2, 0x0a /* Public */,
       5,    1,   53,    2, 0x0a /* Public */,
       6,    1,   56,    2, 0x0a /* Public */,
       7,    3,   59,    2, 0x0a /* Public */,
      11,    2,   66,    2, 0x0a /* Public */,
      12,    2,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    8,    9,   10,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    9,   10,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    9,   10,

       0        // eod
};

void RenderController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->objectRotationChanged(); break;
        case 1: _t->zoomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->xTranslateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->yTranslateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->BeginScaledDrag((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 5: _t->ContinueScaledDrag((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 6: _t->EndScaledDrag((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RenderController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RenderController.data,
    qt_meta_data_RenderController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RenderController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RenderController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RenderController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
