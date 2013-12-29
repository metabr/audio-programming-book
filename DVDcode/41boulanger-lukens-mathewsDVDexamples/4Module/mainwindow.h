#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <portaudio.h>
#include <portsf.h>
#include "module.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void init();

    bool soundOn;
    QTimer *timer;


protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void resetSin();
    void changePitch(int pitch);
    void changeAttack(int attack);
    void changeDecay(int decay);
    void changeWave(int wave);
    void changeTime(int time);
    void changeTimer(int checked);
};

#endif // MAINWINDOW_H
