//mainwindow.cpp - source file to handle the UI

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

extern float freq;      //global frequency
extern int table_num;   //global wave shape table number


//constructor -- THINGS START HERE WHEN THE MAINWINDOW IS CREATED
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);      //set up the user interface

    table_num = 0;          //init the table number (sine wave)

    //connect the horizontal slider for frequency and the combo box for wave shape
    connect(ui->vsFreq, SIGNAL(valueChanged(int)), this, SLOT(changeFreq(int)));
    connect(ui->cbWaveShape, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWaveShape(int)));
}

//slot to change wave shapes (connected to index change of ui->cbWaveShape
void MainWindow::changeWaveShape(int shape)
{
    table_num = shape;      //set the table number to the combo box index
}

//slot connected to the value change of ui->vsFreq
void MainWindow::changeFreq(int cFreq)
{
    freq = cFreq;       //set the frequency as the value of ui->vsFreq

    ui->lbFreq->setText(QString::number(ui->vsFreq->value())); //set the text of our Frequency label to the value of the slider
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//built in function to handle some event changes
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
