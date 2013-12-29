#include <QtGui/QApplication>
#include "mainwindow.h"

bool sinReset;
int sinPitch = 440;
float sinAttack = .1;
float sinDecay = .2;
int sinWave = 1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Module module;
    MainWindow w;
    w.show();
    return a.exec();
}
