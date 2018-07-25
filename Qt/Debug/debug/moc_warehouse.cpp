/****************************************************************************
** Meta object code from reading C++ file 'warehouse.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../warehouse.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'warehouse.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_warehouse_t {
    QByteArrayData data[6];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_warehouse_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_warehouse_t qt_meta_stringdata_warehouse = {
    {
QT_MOC_LITERAL(0, 0, 9), // "warehouse"
QT_MOC_LITERAL(1, 10, 20), // "wrhouse_close_signal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "wrhouse_refresh_slot"
QT_MOC_LITERAL(4, 53, 18), // "wrhouse_add_record"
QT_MOC_LITERAL(5, 72, 8) // "record_t"

    },
    "warehouse\0wrhouse_close_signal\0\0"
    "wrhouse_refresh_slot\0wrhouse_add_record\0"
    "record_t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_warehouse[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void warehouse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        warehouse *_t = static_cast<warehouse *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wrhouse_close_signal(); break;
        case 1: _t->wrhouse_refresh_slot(); break;
        case 2: _t->wrhouse_add_record((*reinterpret_cast< record_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (warehouse::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&warehouse::wrhouse_close_signal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject warehouse::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_warehouse.data,
      qt_meta_data_warehouse,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *warehouse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *warehouse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_warehouse.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int warehouse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void warehouse::wrhouse_close_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
