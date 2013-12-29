#include "rawmode.h"
#include "ui_rawmode.h"
#include <stdio.h>
#include <QTimer>

RawMode::RawMode(QWidget *parent, int inDev, int outDev) :
    QDialog(parent),
    ui(new Ui::RawMode)
{
    ui->setupUi(this);

    sleep(2);

    init(inDev, outDev);
    makeConnections();
}

void RawMode::makeConnections()
{
    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(quitOut()));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateMessages()));
    mTimer->start(0);
}

void RawMode::init(int inDev, int outDev)
{
    Pm_Initialize();
    //const PmDeviceInfo *info;
    //info = Pm_GetDeviceInfo(inDev);
    Pm_OpenInput(&inStream, inDev, NULL, 512, NULL, NULL);
    Pm_OpenOutput(&outStream, outDev, NULL, 512, NULL, NULL, 0);

    event = (PmEvent *)malloc(sizeof(PmEvent));

    int i;
    char* temp;
    temp = (char*)malloc(sizeof(char)*32);

    for(i = 0; i < 127; i++)
    {
        sprintf(temp, "%d", i+1);
        ui->cbB1X->addItem(temp);
        ui->cbB1Y->addItem(temp);
        ui->cbB1Z->addItem(temp);
        ui->cbB2X->addItem(temp);
        ui->cbB2Y->addItem(temp);
        ui->cbB2Z->addItem(temp);
    }
    free(temp);
}

void RawMode::quitOut()
{
    mTimer->disconnect();
    sleep(2);
    close();
}

RawMode::~RawMode()
{
    delete ui;
}

void RawMode::changeEvent(QEvent *e)
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

void RawMode::updateMessages()
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
        case 0xa0:
            ad1 = data2 * 128;
            break;

        case 0xa1:
            ad2 = data2 * 128;
            break;

        case 0xa2:
            ad3 = data2 * 128;
            break;

        case 0xa3:
            ad4 = data2 * 128;
            break;

        case 0xa4:
            ad5 = data2 * 128;
            break;

        case 0xa5:
            ad6 = data2 * 128;
            break;

        case 0xa6:
            ad7 = data2 * 128;
            break;

        case 0xa7:
            ad8 = data2 * 128;
            break;
        }
        x1 = (float)ad8 / (float)(ad8 + ad2) * 153 - 10;
        y1 = (float)ad6 / (float)(ad6 + ad4) * 163 - 15;
        //z1 = (float)ad8 +
        x2 = (float)ad7 / (float)(ad7 + ad1) * 161 - 23;
        y2 = (float)ad5 / (float)(ad5 + ad3) * 174 - 27;

        sprintf(temp, "%.1f", x1);
        ui->leValue1->setText(temp);
        sprintf(temp, "%.1f", y1);
        ui->leValue2->setText(temp);
        sprintf(temp, "%.1f", x2);
        ui->leValue4->setText(temp);
        sprintf(temp, "%.1f", y2);
        ui->leValue5->setText(temp);
    }

    free(temp);
    free(temp2);
}
