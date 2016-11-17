/****************************************************************************
** Meta object code from reading C++ file 'UART.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/UART.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UART.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UART_t {
    QByteArrayData data[9];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UART_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UART_t qt_meta_stringdata_UART = {
    {
QT_MOC_LITERAL(0, 0, 4), // "UART"
QT_MOC_LITERAL(1, 5, 6), // "dataIn"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 4), // "send"
QT_MOC_LITERAL(4, 18, 11), // "handleError"
QT_MOC_LITERAL(5, 30, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(6, 59, 15), // "serialPortError"
QT_MOC_LITERAL(7, 75, 12), // "isSerialOpen"
QT_MOC_LITERAL(8, 88, 6) // "parent"

    },
    "UART\0dataIn\0\0send\0handleError\0"
    "QSerialPort::SerialPortError\0"
    "serialPortError\0isSerialOpen\0parent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UART[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       2,   44, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    1,   36,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    0,   39,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // methods: parameters
    QMetaType::Bool,

 // constructors: parameters
    0x80000000 | 2, QMetaType::QObjectStar,    8,
    0x80000000 | 2,

 // constructors: name, argc, parameters, tag, flags
       0,    1,   40,    2, 0x0e /* Public */,
       0,    0,   43,    2, 0x2e /* Public | MethodCloned */,

       0        // eod
};

void UART::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { UART *_r = new UART((*reinterpret_cast< QObject*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { UART *_r = new UART();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        UART *_t = static_cast<UART *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataIn(); break;
        case 1: _t->send(); break;
        case 2: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 3: { bool _r = _t->isSerialOpen();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject UART::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UART.data,
      qt_meta_data_UART,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UART::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UART::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UART.stringdata0))
        return static_cast<void*>(const_cast< UART*>(this));
    return QObject::qt_metacast(_clname);
}

int UART::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
