#include "matrixmode.h"
#include "ui_matrixmode.h"
#include <QComboBox>
#include <QTimer>

MatrixMode::MatrixMode(QWidget *parent, int inDev, int outDev) :
    QDialog(parent),
    ui(new Ui::MatrixMode)
{
    ui->setupUi(this);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMessages()));
    timer->start(.01);

    ui->cbOut1->addItem("Note Off");
    ui->cbOut1->addItem("Note On");
    ui->cbOut1->addItem("Poly Aftertouch");
    ui->cbOut1->addItem("Control Change");
    ui->cbOut1->addItem("Program Change");
    ui->cbOut1->addItem("Channel Pressure");
    ui->cbOut1->addItem("Pitchbend");

    ui->cbOut2->addItem("Note Off");
    ui->cbOut2->addItem("Note On");
    ui->cbOut2->addItem("Poly Aftertouch");
    ui->cbOut2->addItem("Control Change");
    ui->cbOut2->addItem("Program Change");
    ui->cbOut2->addItem("Channel Pressure");
    ui->cbOut2->addItem("Pitchbend");
    ui->cbOut2->setCurrentIndex(1);

    ui->cbOut3->addItem("Note Off");
    ui->cbOut3->addItem("Note On");
    ui->cbOut3->addItem("Poly Aftertouch");
    ui->cbOut3->addItem("Control Change");
    ui->cbOut3->addItem("Program Change");
    ui->cbOut3->addItem("Channel Pressure");
    ui->cbOut3->addItem("Pitchbend");
    ui->cbOut3->setCurrentIndex(2);

    ui->cbOut4->addItem("Note Off");
    ui->cbOut4->addItem("Note On");
    ui->cbOut4->addItem("Poly Aftertouch");
    ui->cbOut4->addItem("Control Change");
    ui->cbOut4->addItem("Program Change");
    ui->cbOut4->addItem("Channel Pressure");
    ui->cbOut4->addItem("Pitchbend");
    ui->cbOut4->setCurrentIndex(3);

    ui->cbOut5->addItem("Note Off");
    ui->cbOut5->addItem("Note On");
    ui->cbOut5->addItem("Poly Aftertouch");
    ui->cbOut5->addItem("Control Change");
    ui->cbOut5->addItem("Program Change");
    ui->cbOut5->addItem("Channel Pressure");
    ui->cbOut5->addItem("Pitchbend");
    ui->cbOut5->setCurrentIndex(4);

    ui->cbOut6->addItem("Note Off");
    ui->cbOut6->addItem("Note On");
    ui->cbOut6->addItem("Poly Aftertouch");
    ui->cbOut6->addItem("Control Change");
    ui->cbOut6->addItem("Program Change");
    ui->cbOut6->addItem("Channel Pressure");
    ui->cbOut6->addItem("Pitchbend");
    ui->cbOut6->setCurrentIndex(5);

    ui->cbOut7->addItem("Note Off");
    ui->cbOut7->addItem("Note On");
    ui->cbOut7->addItem("Poly Aftertouch");
    ui->cbOut7->addItem("Control Change");
    ui->cbOut7->addItem("Program Change");
    ui->cbOut7->addItem("Channel Pressure");
    ui->cbOut7->addItem("Pitchbend");
    ui->cbOut7->setCurrentIndex(6);

    ui->leOffset1->setText("0");
    ui->leScale1->setText("1");
    ui->leOffset2->setText("0");
    ui->leScale2->setText("1");
    ui->leOffset3->setText("0");
    ui->leScale3->setText("1");
    ui->leOffset4->setText("0");
    ui->leScale4->setText("1");
    ui->leOffset5->setText("0");
    ui->leScale5->setText("1");
    ui->leOffset6->setText("0");
    ui->leScale6->setText("1");
    ui->leOffset7->setText("0");
    ui->leScale7->setText("1");

    Pm_Initialize();
    const PmDeviceInfo *info;
    info = Pm_GetDeviceInfo(inDev);
    Pm_OpenInput(&inStream, inDev, NULL, 512, NULL, NULL);
    Pm_OpenOutput(&outStream, outDev, NULL, 512, NULL, NULL, 0);

    event = (PmEvent *)malloc(sizeof(PmEvent));
}

void MatrixMode::updateMessages()
{
    long status, data1, data2;

    char* temp;
    char* temp2;
    temp = (char*)malloc(sizeof(char)*32);
    temp2 = (char*)malloc(sizeof(char)*32);

    if(Pm_Poll(inStream)) //was there an event?
    {
        Pm_Read(inStream, event, sizeof(long));

        //translate the message to longs
        status = Pm_MessageStatus(event->message);
        data1 = Pm_MessageData1(event->message);
        data2 = Pm_MessageData2(event->message);

        switch(status)
        {
        case MD_NOTEOFF:
            sprintf(temp, "%d", data1);
            ui->leValue1->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale1->text().toLatin1()) * atof(ui->leValue1->text().toLatin1()) + atof(ui->leOffset1->text().toLatin1()));
            ui->leOutput1->setText((QString)temp2);
            break;
        case MD_NOTEON:
            sprintf(temp, "%d", data2);
            ui->leValue2->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale2->text().toLatin1()) * atof(ui->leValue2->text().toLatin1()) + atof(ui->leOffset2->text().toLatin1()));
            ui->leOutput2->setText((QString)temp2);
            break;
        case MD_POLY:
            sprintf(temp, "%d", data2);
            ui->leValue3->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale3->text().toLatin1()) * atof(ui->leValue3->text().toLatin1()) + atof(ui->leOffset3->text().toLatin1()));
            ui->leOutput3->setText((QString)temp2);
            break;
        case MD_CONTROL:
            sprintf(temp, "%d", data2);
            ui->leValue4->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale4->text().toLatin1()) * atof(ui->leValue4->text().toLatin1()) + atof(ui->leOffset4->text().toLatin1()));
            ui->leOutput4->setText((QString)temp2);
            break;
        case MD_PROGRAM:
            sprintf(temp, "%d", data2);
            ui->leValue5->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale5->text().toLatin1()) * atof(ui->leValue5->text().toLatin1()) + atof(ui->leOffset5->text().toLatin1()));
            ui->leOutput5->setText((QString)temp2);
            break;
        case MD_CHANNEL:
            sprintf(temp, "%d", data2);
            ui->leValue6->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale6->text().toLatin1()) * atof(ui->leValue6->text().toLatin1()) + atof(ui->leOffset6->text().toLatin1()));
            ui->leOutput6->setText((QString)temp2);
            break;
        case MD_PITCH:
            sprintf(temp, "%d", data2);
            ui->leValue7->setText((QString)temp);

            sprintf(temp2, "%.2f", atof(ui->leScale7->text().toLatin1()) * atof(ui->leValue7->text().toLatin1()) + atof(ui->leOffset7->text().toLatin1()));
            ui->leOutput7->setText((QString)temp2);
            break;
        }
    }

    free(temp);
    free(temp2);
}

MatrixMode::~MatrixMode()
{
    delete ui;
}

void MatrixMode::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
