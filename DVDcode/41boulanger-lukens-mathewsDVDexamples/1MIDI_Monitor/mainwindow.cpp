//mainwindow.cpp - source file to handle everything that happens in the MainWindow

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>          //for Timers
#include <QString>         //for QStrings

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  //this is a built in function to setup the UI
    init();             //init MIDI and variables

    QTimer *timer = new QTimer;         //create a timer object

    //the connect function allows the programmer to connect certain signals(mouse click, timer time-out, etc) to a slot(a special type of function)
    //in this case, we have connected the timer's timeout slot (when it cycles through the amount of time we define later)
    //to the slot we have defined in the header called updateMidi().
    //This will allow us to poll for MIDI messages every millisecond
    //the arguments for the function are sender object, signal, receiver object, slot
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMidi()));

    timer->start(1);        //start the timer with a 1ms cycle

    //these connects wait for the combo boxes to change and pass the index to their corresponding methods
    connect(ui->cbMidiIn, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMidiIn(int)));
    connect(ui->cbMidiOut, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMidiOut(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//this function initializes PortMIDI and puts all of the in and out devices into combo boxes
void MainWindow::init()
{
    int i, cnt, outOpen = 0; //counting variables
    totalInDevices = 0;      //keep track of the total input devices

    event = (PmEvent *)malloc(sizeof(PmEvent)); //allocate space for a MIDI event

    const PmDeviceInfo *info;                   //MIDI device information struct

    Pm_Initialize();	//Init PortMIDI

    if((cnt = Pm_CountDevices()))	//return the number of devices
    {
        for(i=0; i < cnt; i++)          //loop through the number of devices
        {
            info = Pm_GetDeviceInfo(i); //gets device info
            if(info->input)             //if the device is available for input
            {
                ui->cbMidiIn->addItem(info->name, NULL);            //add the device name to MIDI In combo box
                if(!totalInDevices)
                {
                    Pm_OpenInput(&inStream, i, NULL, 512, NULL, NULL);  //Open an input stream to that device if it's the first one

                    //since all of our widgets are on the UI we get them from the ui instance
                    //and we're using a function that belongs to lvMessages (which is an instance of a List Widget).
                    //so ui->Message->insertItem is a function that inserts an item where ever in the list you want (0 being the top).
                    //the function tr() is a translate function. This will make it so that if a user has a Spanish version of OS X installed
                    //this will attempt to translate the text into Spanish.
                    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for input.");  //Let the user know a device was opened
                }
                totalInDevices++;   //increment the number of input devices (This is so we can point to the right output device later)
            }
            if(info->output)            //if the device is available for output
            {
                ui->cbMidiOut->addItem(info->name, NULL);   //add the device name to the MIDI Out combo box
                if(!outOpen)
                {
                    Pm_OpenOutput(&outStream, i, NULL, 512, NULL, NULL, 0);        //open the device if an output device hasn't been opened yet
                    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for output."); //let the user know a device was opened
                }
                outOpen = 1;        //so we don't open a second device
            }
        }
    }
}

void MainWindow::updateMidi()
{
    long status, data1, data2;

    if(Pm_Poll(inStream)) //was there an event?
    {
        Pm_Read(inStream, event, sizeof(long));                     //read in the event and store it in the variable event
        if(ui->lvMessages->count() > 200) ui->lvMessages->clear();  //if there are 200 messages in the list widget then clear it out

        //translate the message to longs
        status = Pm_MessageStatus(event->message);  //MIDI status byte
        data1 = Pm_MessageData1(event->message);    //MIDI data byte 1
        data2 = Pm_MessageData2(event->message);    //MIDI data byte 2

        //print the input message
        //the QString::number() function converts a number into a string represented in any base
        //so QString::number(status, 16) converts the status byte to hexidecimal (string)
        //QString::number(data1) converts the number into base 10 by default (still a string, this is so we can print it).
        QString message = "MIDI Input Event: status: 0x" + QString::number(status, 16) + "  data byte 1: " + QString::number(data1) + "  data byte 2: " + QString::number(data2);
        ui->lvMessages->insertItem(0, message);

        //if the thru checkbox is checked, the print the message and write the input message to the output stream
        //the checkState() function returns whether or not the checkbox is checked
        //it returns Qt::Checked if it is, and Qt::Unchecked if not.
        if(ui->cbThru->checkState() == Qt::Checked)
        {
            //write the output message to the list widget
            QString message = "MIDI Output Event: status: 0x" + QString::number(status, 16) + "  data byte 1: " + QString::number(data1) + "  data byte 2: " + QString::number(data2);
            ui->lvMessages->insertItem(0, message);

            //write the message to the output stream
            Pm_Write(outStream, event, 1);
        }
    }
}

void MainWindow::changeMidiIn(int device)
{
    Pm_Close(inStream);     //close the input device
    Pm_OpenInput(&inStream, device, NULL, 512, NULL, NULL);     //open the new input device

    const PmDeviceInfo *info = Pm_GetDeviceInfo(device);        //get the device info so we can print the name

    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for input.");  //Let the user know a device was opened
}

void MainWindow::changeMidiOut(int device)
{
    Pm_Close(outStream);    //close the output device

    //open the new output device
    //using device + totalInDevices since output devices come after input devices
    //so the combo box index + totalInDevices will give us the actual device number we are looking for
    Pm_OpenOutput(&outStream, device + totalInDevices, NULL, 512, NULL, NULL, 0);

    const PmDeviceInfo *info = Pm_GetDeviceInfo(device + totalInDevices);        //get the device info so we can print the name

    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for output.");  //Let the user know a device was opened
}


//built-in function to handle some event changes
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
