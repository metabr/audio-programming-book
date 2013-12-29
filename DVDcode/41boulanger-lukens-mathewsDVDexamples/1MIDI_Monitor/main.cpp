#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //creates an instance of a Qt application (with arguments)
    MainWindow w;               //creates an instance of a MainWindow (at this point, the constructor is called - the function MainWindow::MainWindow() in MainWindow.cpp)
    w.show();                   //show the window
    return a.exec();            //exec() returns when the MainWindow is exited out of (so quit when the window is closed)
}
