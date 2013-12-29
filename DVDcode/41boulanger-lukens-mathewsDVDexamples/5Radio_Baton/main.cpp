#include <QtGui/QApplication>
#include "mainwindow.h"
#include "editwindow.h"

int main(int argc, char *argv[])
{
    //create the application
    QApplication a(argc, argv);

    //create an instance of MainWindow
    MainWindow w;
    //show the window
    w.show();
    //execute the applicaton
    return a.exec();
}
