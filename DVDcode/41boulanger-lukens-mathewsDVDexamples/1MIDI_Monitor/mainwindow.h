#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "portmidi.h"

//the User Interface
namespace Ui {
    class MainWindow;
}

//the class for our MainWindow
class MainWindow : public QMainWindow {
    Q_OBJECT

    //public classes and variables
public:
    MainWindow(QWidget *parent = 0);    //constructor (this is called when the MainWindow is created)
    ~MainWindow();                      //destructor (this calls when the MainWindow goes away)
    void init();                    //initialization function

    int totalInDevices;             //keep track of the total input MIDI devices

    PmEvent* event;                 //MIDI event
    PortMidiStream *inStream;       //Input MIDI stream
    PortMidiStream *outStream;      //Output MIDI stream

protected:
    void changeEvent(QEvent *e);    //built in to handle resizing

private:
    Ui::MainWindow *ui;             //instance of the UI

private slots:
    void updateMidi();              //user defined SLOT called updateMidi (used with the timer in the .cpp file)
    void changeMidiIn(int device);  //if the user changes the MIDI Input device
    void changeMidiOut(int device); //if the user changes the MIDI output device
};

#endif // MAINWINDOW_H
