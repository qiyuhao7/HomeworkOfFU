/****************************************************************************
** Meta object code from reading C++ file 'renderingwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../renderingwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderingwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RenderingWidget_t {
    QByteArrayData data[18];
    char stringdata[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_RenderingWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_RenderingWidget_t qt_meta_stringdata_RenderingWidget = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 8),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 12),
QT_MOC_LITERAL(4, 39, 13),
QT_MOC_LITERAL(5, 53, 8),
QT_MOC_LITERAL(6, 62, 9),
QT_MOC_LITERAL(7, 72, 11),
QT_MOC_LITERAL(8, 84, 14),
QT_MOC_LITERAL(9, 99, 2),
QT_MOC_LITERAL(10, 102, 13),
QT_MOC_LITERAL(11, 116, 13),
QT_MOC_LITERAL(12, 130, 10),
QT_MOC_LITERAL(13, 141, 16),
QT_MOC_LITERAL(14, 158, 13),
QT_MOC_LITERAL(15, 172, 7),
QT_MOC_LITERAL(16, 180, 20),
QT_MOC_LITERAL(17, 201, 15)
    },
    "RenderingWidget\0meshInfo\0\0operatorInfo\0"
    "SetBackground\0ReadMesh\0WriteMesh\0"
    "LoadTexture\0CheckDrawPoint\0bv\0"
    "CheckDrawEdge\0CheckDrawFace\0CheckLight\0"
    "CheckDrawTexture\0CheckDrawAxes\0Uniform\0"
    "WeightedLeastSquares\0ShapePreserving\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   89,    2, 0x06,
       3,    1,   96,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,   99,    2, 0x0a,
       5,    0,  100,    2, 0x0a,
       6,    0,  101,    2, 0x0a,
       7,    0,  102,    2, 0x0a,
       8,    1,  103,    2, 0x0a,
      10,    1,  106,    2, 0x0a,
      11,    1,  109,    2, 0x0a,
      12,    1,  112,    2, 0x0a,
      13,    1,  115,    2, 0x0a,
      14,    1,  118,    2, 0x0a,
      15,    0,  121,    2, 0x0a,
      16,    0,  122,    2, 0x0a,
      17,    0,  123,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RenderingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderingWidget *_t = static_cast<RenderingWidget *>(_o);
        switch (_id) {
        case 0: _t->meshInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->operatorInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->SetBackground(); break;
        case 3: _t->ReadMesh(); break;
        case 4: _t->WriteMesh(); break;
        case 5: _t->LoadTexture(); break;
        case 6: _t->CheckDrawPoint((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->CheckDrawEdge((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->CheckDrawFace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->CheckLight((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->CheckDrawTexture((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->CheckDrawAxes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->Uniform(); break;
        case 13: _t->WeightedLeastSquares(); break;
        case 14: _t->ShapePreserving(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RenderingWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderingWidget::meshInfo)) {
                *result = 0;
            }
        }
        {
            typedef void (RenderingWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderingWidget::operatorInfo)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject RenderingWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_RenderingWidget.data,
      qt_meta_data_RenderingWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *RenderingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RenderingWidget.stringdata))
        return static_cast<void*>(const_cast< RenderingWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int RenderingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void RenderingWidget::meshInfo(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RenderingWidget::operatorInfo(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
