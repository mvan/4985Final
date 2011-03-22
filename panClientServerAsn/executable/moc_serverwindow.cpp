/****************************************************************************
** Meta object code from reading C++ file 'serverwindow.h'
**
** Created: Tue Feb 22 09:45:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../asn2/serverwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   45,   13,   13, 0x0a,
      85,   13,   13,   13, 0x0a,
     119,  106,   13,   13, 0x0a,
     150,  137,   13,   13, 0x0a,
     181,  176,   13,   13, 0x2a,
     226,  203,   13,   13, 0x0a,
     260,  246,   13,   13, 0x2a,
     276,   13,   13,   13, 0x08,
     288,   13,   13,   13, 0x08,
     311,  301,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ServerWindow[] = {
    "ServerWindow\0\0fileName\0selectedFile(QString)\0"
    "title,msg\0displayError(QString,QString)\0"
    "cleanStoppedServer()\0clientSocket\0"
    "addClient(SOCKET)\0text,timeout\0"
    "setStatusBar(QString,int)\0text\0"
    "setStatusBar(QString)\0bytesReceived,duration\0"
    "addPacket(uint,int)\0bytesReceived\0"
    "addPacket(uint)\0runServer()\0stopServer()\0"
    "eventLoop\0getSaveFileName(QEventLoop*)\0"
};

const QMetaObject ServerWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ServerWindow,
      qt_meta_data_ServerWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServerWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerWindow))
        return static_cast<void*>(const_cast< ServerWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ServerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectedFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: displayError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: cleanStoppedServer(); break;
        case 3: addClient((*reinterpret_cast< SOCKET(*)>(_a[1]))); break;
        case 4: setStatusBar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: setStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: addPacket((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: addPacket((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 8: runServer(); break;
        case 9: stopServer(); break;
        case 10: getSaveFileName((*reinterpret_cast< QEventLoop*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ServerWindow::selectedFile(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
