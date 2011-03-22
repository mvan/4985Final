/****************************************************************************
** Meta object code from reading C++ file 'clientprocessthread.h'
**
** Created: Tue Feb 22 09:45:42 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../asn2/clientprocessthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientprocessthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClientProcessThread[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   21,   20,   20, 0x05,
      62,   20,   20,   20, 0x05,
      88,   78,   20,   20, 0x05,
     137,  114,   20,   20, 0x05,
     178,  164,   20,   20, 0x25,
     201,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     237,  228,  223,   20, 0x08,
     255,  228,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ClientProcessThread[] = {
    "ClientProcessThread\0\0title,msg\0"
    "errorOccurred(QString,QString)\0"
    "serverStopped()\0eventLoop\0"
    "receivedData(QEventLoop*)\0"
    "bytesReceived,duration\0"
    "receivedMoreData(uint,int)\0bytesReceived\0"
    "receivedMoreData(uint)\0changeStatus(QString)\0"
    "bool\0filename\0openFile(QString)\0"
    "getFileName(QString)\0"
};

const QMetaObject ClientProcessThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ClientProcessThread,
      qt_meta_data_ClientProcessThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClientProcessThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClientProcessThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClientProcessThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClientProcessThread))
        return static_cast<void*>(const_cast< ClientProcessThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ClientProcessThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: errorOccurred((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: serverStopped(); break;
        case 2: receivedData((*reinterpret_cast< QEventLoop*(*)>(_a[1]))); break;
        case 3: receivedMoreData((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: receivedMoreData((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 5: changeStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: { bool _r = openFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: getFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ClientProcessThread::errorOccurred(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientProcessThread::serverStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ClientProcessThread::receivedData(QEventLoop * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientProcessThread::receivedMoreData(unsigned int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 5
void ClientProcessThread::changeStatus(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
