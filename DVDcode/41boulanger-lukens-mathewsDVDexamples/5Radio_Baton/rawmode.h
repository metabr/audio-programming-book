#ifndef RAWMODE_H
#define RAWMODE_H

#include <QDialog>
#include "portmidi.h"
#include "porttime.h"

namespace Ui {
    class RawMode;
}

class RawMode : public QDialog {
    Q_OBJECT
public:
    RawMode(QWidget *parent = 0, int inDev = 0, int outDev = 0);
    ~RawMode();

    PortMidiStream *inStream;
    PortMidiStream *outStream;
    PmEvent *event;

    int ad1, ad2, ad3, ad4, ad5, ad6, ad7, ad8;
    float x1, x2, y1, y2, z1, z2;

    QTimer *mTimer;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RawMode *ui;
    void init(int inDev, int outDev);
    void makeConnections();

private slots:
    void updateMessages();
    void quitOut();
};

#endif // RAWMODE_H
