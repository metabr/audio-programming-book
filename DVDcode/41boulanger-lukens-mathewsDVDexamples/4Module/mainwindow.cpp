#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "module.h"
#include <QTimer>
#include <QString>

extern bool sinReset;
extern int sinPitch;
extern float sinAttack;
extern float sinDecay;
extern int sinWave;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Module data;

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->vsAttack->setValue(10);
    ui->vsDecay->setValue(20);
    ui->vsPitch->setValue(440);
    connect(ui->pbSinReset, SIGNAL(clicked()), this, SLOT(resetSin()));
    connect(ui->vsPitch, SIGNAL(valueChanged(int)), this, SLOT(changePitch(int)));
    connect(ui->vsAttack, SIGNAL(valueChanged(int)), this, SLOT(changeAttack(int)));
    connect(ui->vsDecay, SIGNAL(valueChanged(int)), this, SLOT(changeDecay(int)));
    connect(ui->cbWave, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWave(int)));
    connect(ui->vsTime, SIGNAL(valueChanged(int)), this, SLOT(changeTime(int)));
    connect(ui->cbAutoPlay, SIGNAL(stateChanged(int)), this, SLOT(changeTimer(int)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(resetSin()));
    //timer->start(75);
}

void MainWindow::changeTimer(int checked)
{
    if(checked) timer->start(ui->vsTime->value());
    else timer->stop();
}

void MainWindow::changeTime(int time)
{
    ui->lbAutoTime->setText(QString::number(time));

    if(ui->cbAutoPlay->checkState())
    {
        //timer->stop();
        timer->start(time);
    }
}

void MainWindow::resetSin()
{
    sinReset = 1;
}

void MainWindow::changePitch(int pitch)
{
    sinPitch = pitch;
    ui->lbPitch->setText(QString::number(pitch));
}

void MainWindow::changeAttack(int attack)
{
    sinAttack = (float)attack / 100;
    ui->lbAttack->setText(QString::number(sinAttack));
}

void MainWindow::changeDecay(int decay)
{
    sinDecay = (float)decay / 100;
    ui->lbDecay->setText(QString::number(sinDecay));
}

void MainWindow::changeWave(int wave)
{
    sinWave = wave + 1;
}

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
