#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QFileDialog>
#include <QDialog>
#include "portmidi.h"
#include "porttime.h"
#include <QStringList>
#include <string.h>
#include <QByteArray>
#include <QDir>
#include <rawmode.h>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initMidi();
    void checkAllFilters();
    void startMidiTimer();
    void makeConnections();
    void changeFilter();

    PortMidiStream *inStream;
    PortMidiStream *outStream;
    int inDev, outDev;
    PmEvent *event;
    int totalInDevices;
    int currentMode;

    bool ccFilt;
    bool noteFilt;
    bool polyFilt;
    bool chanFilt;
    bool pitchFilt;
    bool progFilt;
    bool sysFilt;
    bool realFilt;

    bool inOpen;
    bool outOpen;
    bool mThru;
    bool condClockOn;
    bool dispMidi;
    bool matrixMode;
    bool updating;

    QMutex mutex;

    QString fileName;

    QTimer *mTimer;
    bool mTimerOn;

    RawMode* tlRaw;


    //STUFF TAKEN FROM COND
    //cond2 functions
    void midiparser(unsigned char byte0, unsigned char byte1, unsigned char byte2);
    void initialization(void);
    void initializecond(void);
    int keyvelfun(int v);
    void xformandsend(void);
    void pcc(void);
    void loadplay(void);
    void playrecord(void);
    void readscorez(void);
    void stkcontrol(void);
    void playscorefile(void);
    void playruns(void);
    void processbuttoncommand(void);
    void processfeetcommand(void);
    void printbuttonsforscore(void);
    void midisend(int status, int channel, int byte1, int byte2);
    void midisendprogramchange(int channel, int byte1);
    void printtimbre(int timb);
    void playrhythmicon(void);

    //portmidi functions
    void setdevices(void);
    int get_number(char *prompt);
    void startmidi(void);
    void openMidi();
    void closeMidi();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void updateMessages();
    void changeMidiIn();
    void changeMidiOut();
    void fControllerChange();
    void fNote();
    void fPolyAftertouch();
    void fMonoAftertouch();
    void fPitchBend();
    void fProgramChange();
    void fSysEx();
    void fRealTime();
    void modeChange();
    void openFile();
    void compile();
    void showEditWindow();
    void updateDevices();
    void thru();
    void startCond();
    void condClock();
    void displayMidi();
    void stopCond();
    void startMatrixMode();
    void startRawMode();
    void stopMidiTimer();
};

#endif // MAINWINDOW_H
