/****************************************************************************
** Meta object code from reading C++ file 'startserverdlg.h'
**
** Created: Tue Feb 22 09:45:40 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../asn2/startserverdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'startserverdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_startServerDlg[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   16,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_startServerDlg[] = {
    "startServerDlg\0\0bool\0startServer()\0"
};

const QMetaObject startServerDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_startServerDlg,
      qt_meta_data_startServerDlg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &startServerDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *startServerDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *startServerDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_startServerDlg))
        return static_cast<void*>(const_cast< startServerDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int startServerDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = startServer();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
