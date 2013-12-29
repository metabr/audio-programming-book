#ifndef MATRIXMODE_H
#define MATRIXMODE_H

#include <QDialog>
#include "portmidi.h"
#include "porttime.h"
#include <stdlib.h>

#define MD_NOTEOFF      0x80
#define MD_NOTEON       0x90
#define MD_POLY         0xA0
#define MD_CONTROL      0xB0
#define MD_PROGRAM      0xC0
#define MD_CHANNEL      0xD0
#define MD_PITCH        0xE0

namespace Ui {
    class MatrixMode;
}

class MatrixMode : public QDialog {
    Q_OBJECT
public:
    MatrixMode(QWidget *parent, int inDev, int outDev);
    ~MatrixMode();

    PortMidiStream *inStream;
    PortMidiStream *outStream;
    PmEvent *event;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MatrixMode *ui;

private slots:
    void updateMessages();
};

#endif // MATRIXMODE_H
