/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Jun 28 18:54:06 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      29,   11,   11,   11, 0x08,
      44,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      80,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     106,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     137,   11,   11,   11, 0x08,
     154,   11,   11,   11, 0x08,
     163,   11,   11,   11, 0x08,
     175,   11,   11,   11, 0x08,
     188,   11,   11,   11, 0x08,
     199,   11,   11,   11, 0x08,
     209,   11,   11,   11, 0x08,
     226,   11,   11,   11, 0x08,
     242,   11,   11,   11, 0x08,
     249,   11,   11,   11, 0x08,
     261,   11,   11,   11, 0x08,
     273,   11,   11,   11, 0x08,
     287,   11,   11,   11, 0x08,
     298,   11,   11,   11, 0x08,
     316,   11,   11,   11, 0x08,
     331,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0updateMessages()\0"
    "changeMidiIn()\0changeMidiOut()\0"
    "fControllerChange()\0fNote()\0"
    "fPolyAftertouch()\0fMonoAftertouch()\0"
    "fPitchBend()\0fProgramChange()\0fSysEx()\0"
    "fRealTime()\0modeChange()\0openFile()\0"
    "compile()\0showEditWindow()\0updateDevices()\0"
    "thru()\0startCond()\0condClock()\0"
    "displayMidi()\0stopCond()\0startMatrixMode()\0"
    "startRawMode()\0stopMidiTimer()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateMessages(); break;
        case 1: changeMidiIn(); break;
        case 2: changeMidiOut(); break;
        case 3: fControllerChange(); break;
        case 4: fNote(); break;
        case 5: fPolyAftertouch(); break;
        case 6: fMonoAftertouch(); break;
        case 7: fPitchBend(); break;
        case 8: fProgramChange(); break;
        case 9: fSysEx(); break;
        case 10: fRealTime(); break;
        case 11: modeChange(); break;
        case 12: openFile(); break;
        case 13: compile(); break;
        case 14: showEditWindow(); break;
        case 15: updateDevices(); break;
        case 16: thru(); break;
        case 17: startCond(); break;
        case 18: condClock(); break;
        case 19: displayMidi(); break;
        case 20: stopCond(); break;
        case 21: startMatrixMode(); break;
        case 22: startRawMode(); break;
        case 23: stopMidiTimer(); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
