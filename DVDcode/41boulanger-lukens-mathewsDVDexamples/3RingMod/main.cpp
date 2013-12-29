#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ring.h"

float freq;     //global frequency
int table_num;  //global wave shape table number

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);     //create the application
    MainWindow w;                   //make an instance of MainWindow
    Ring ring;                      //make an instance of Ring (all of the portAudio stuff is in here)
    w.show();                       //show the window
    return a.exec();                //execute the window and return when the window closes
}
