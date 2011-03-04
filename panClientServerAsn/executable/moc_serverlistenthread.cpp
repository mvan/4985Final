/****************************************************************************
** Meta object code from reading C++ file 'serverlistenthread.h'
**
** Created: Tue Feb 22 09:45:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../asn2/serverlistenthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverlistenthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerListenThread[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   20,   19,   19, 0x05,
      61,   19,   19,   19, 0x05,
      90,   77,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ServerListenThread[] = {
    "ServerListenThread\0\0title,msg\0"
    "errorOccurred(QString,QString)\0"
    "serverStopped()\0clientSocket\0"
    "clientConnected(SOCKET)\0"
};

const QMetaObject ServerListenThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ServerListenThread,
      qt_meta_data_ServerListenThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServerListenThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServerListenThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServerListenThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerListenThread))
        return static_cast<void*>(const_cast< ServerListenThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ServerListenThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: errorOccurred((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: serverStopped(); break;
        case 2: clientConnected((*reinterpret_cast< SOCKET(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ServerListenThread::errorOccurred(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerListenThread::serverStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ServerListenThread::clientConnected(SOCKET _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
