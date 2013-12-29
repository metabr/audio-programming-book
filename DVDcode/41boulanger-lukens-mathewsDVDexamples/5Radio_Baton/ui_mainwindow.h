/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Jun 28 18:28:38 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Configuration;
    QAction *actionOpen_Configuration;
    QAction *actionQuit;
    QWidget *centralwidget;
    QPushButton *pbEditFile;
    QCheckBox *cbNote;
    QCheckBox *cbPolyAftertouch;
    QCheckBox *cbControllerChange;
    QCheckBox *cbProgramChange;
    QCheckBox *cbMonoAftertouch;
    QCheckBox *cbPitchBend;
    QLabel *lConductDisplay;
    QLabel *lMode;
    QComboBox *cbMode;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lMidiIn;
    QLabel *lMidiOut;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cbMidiIn;
    QComboBox *cbMidiOut;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lMessages;
    QCheckBox *cbDisplayMidi;
    QLabel *lFilters;
    QListWidget *lvCondDisplay;
    QPushButton *pbCompile;
    QCheckBox *cbSysEx;
    QCheckBox *cbRealTime;
    QPushButton *pbUpdateDevices;
    QLabel *lConduct;
    QPushButton *pbConductOpen;
    QLabel *lConductFile;
    QCheckBox *cbThru;
    QPushButton *pbStartCond;
    QPushButton *pbStopCond;
    QListWidget *lvMessages;
    QPushButton *pbMatrixMode;
    QPushButton *pbRawMode;
    QPushButton *pbKill;
    QMenuBar *menubar;
    QMenu *menuRadio_Baton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionNew_Configuration = new QAction(MainWindow);
        actionNew_Configuration->setObjectName(QString::fromUtf8("actionNew_Configuration"));
        actionOpen_Configuration = new QAction(MainWindow);
        actionOpen_Configuration->setObjectName(QString::fromUtf8("actionOpen_Configuration"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pbEditFile = new QPushButton(centralwidget);
        pbEditFile->setObjectName(QString::fromUtf8("pbEditFile"));
        pbEditFile->setGeometry(QRect(310, 45, 91, 32));
        cbNote = new QCheckBox(centralwidget);
        cbNote->setObjectName(QString::fromUtf8("cbNote"));
        cbNote->setGeometry(QRect(470, 150, 87, 20));
        cbPolyAftertouch = new QCheckBox(centralwidget);
        cbPolyAftertouch->setObjectName(QString::fromUtf8("cbPolyAftertouch"));
        cbPolyAftertouch->setGeometry(QRect(470, 175, 121, 20));
        cbControllerChange = new QCheckBox(centralwidget);
        cbControllerChange->setObjectName(QString::fromUtf8("cbControllerChange"));
        cbControllerChange->setGeometry(QRect(470, 200, 141, 20));
        cbProgramChange = new QCheckBox(centralwidget);
        cbProgramChange->setObjectName(QString::fromUtf8("cbProgramChange"));
        cbProgramChange->setGeometry(QRect(470, 225, 131, 20));
        cbMonoAftertouch = new QCheckBox(centralwidget);
        cbMonoAftertouch->setObjectName(QString::fromUtf8("cbMonoAftertouch"));
        cbMonoAftertouch->setGeometry(QRect(470, 250, 131, 20));
        cbPitchBend = new QCheckBox(centralwidget);
        cbPitchBend->setObjectName(QString::fromUtf8("cbPitchBend"));
        cbPitchBend->setGeometry(QRect(470, 275, 87, 20));
        lConductDisplay = new QLabel(centralwidget);
        lConductDisplay->setObjectName(QString::fromUtf8("lConductDisplay"));
        lConductDisplay->setGeometry(QRect(20, 355, 111, 16));
        lMode = new QLabel(centralwidget);
        lMode->setObjectName(QString::fromUtf8("lMode"));
        lMode->setGeometry(QRect(480, 365, 111, 16));
        cbMode = new QComboBox(centralwidget);
        cbMode->setObjectName(QString::fromUtf8("cbMode"));
        cbMode->setGeometry(QRect(480, 385, 131, 23));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 62, 51));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lMidiIn = new QLabel(layoutWidget);
        lMidiIn->setObjectName(QString::fromUtf8("lMidiIn"));

        verticalLayout->addWidget(lMidiIn);

        lMidiOut = new QLabel(layoutWidget);
        lMidiOut->setObjectName(QString::fromUtf8("lMidiOut"));

        verticalLayout->addWidget(lMidiOut);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(90, 20, 111, 56));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cbMidiIn = new QComboBox(layoutWidget1);
        cbMidiIn->setObjectName(QString::fromUtf8("cbMidiIn"));

        verticalLayout_2->addWidget(cbMidiIn);

        cbMidiOut = new QComboBox(layoutWidget1);
        cbMidiOut->setObjectName(QString::fromUtf8("cbMidiOut"));

        verticalLayout_2->addWidget(cbMidiOut);

        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(20, 125, 491, 23));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lMessages = new QLabel(layoutWidget2);
        lMessages->setObjectName(QString::fromUtf8("lMessages"));

        horizontalLayout_2->addWidget(lMessages);

        cbDisplayMidi = new QCheckBox(layoutWidget2);
        cbDisplayMidi->setObjectName(QString::fromUtf8("cbDisplayMidi"));

        horizontalLayout_2->addWidget(cbDisplayMidi);

        lFilters = new QLabel(layoutWidget2);
        lFilters->setObjectName(QString::fromUtf8("lFilters"));
        lFilters->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_2->addWidget(lFilters);

        lvCondDisplay = new QListWidget(centralwidget);
        lvCondDisplay->setObjectName(QString::fromUtf8("lvCondDisplay"));
        lvCondDisplay->setGeometry(QRect(20, 375, 441, 192));
        pbCompile = new QPushButton(centralwidget);
        pbCompile->setObjectName(QString::fromUtf8("pbCompile"));
        pbCompile->setGeometry(QRect(410, 45, 91, 32));
        cbSysEx = new QCheckBox(centralwidget);
        cbSysEx->setObjectName(QString::fromUtf8("cbSysEx"));
        cbSysEx->setGeometry(QRect(470, 300, 87, 20));
        cbRealTime = new QCheckBox(centralwidget);
        cbRealTime->setObjectName(QString::fromUtf8("cbRealTime"));
        cbRealTime->setGeometry(QRect(470, 325, 87, 20));
        pbUpdateDevices = new QPushButton(centralwidget);
        pbUpdateDevices->setObjectName(QString::fromUtf8("pbUpdateDevices"));
        pbUpdateDevices->setGeometry(QRect(80, 80, 131, 32));
        lConduct = new QLabel(centralwidget);
        lConduct->setObjectName(QString::fromUtf8("lConduct"));
        lConduct->setGeometry(QRect(309, 15, 96, 16));
        pbConductOpen = new QPushButton(centralwidget);
        pbConductOpen->setObjectName(QString::fromUtf8("pbConductOpen"));
        pbConductOpen->setGeometry(QRect(510, 45, 101, 32));
        lConductFile = new QLabel(centralwidget);
        lConductFile->setObjectName(QString::fromUtf8("lConductFile"));
        lConductFile->setGeometry(QRect(413, 15, 181, 16));
        cbThru = new QCheckBox(centralwidget);
        cbThru->setObjectName(QString::fromUtf8("cbThru"));
        cbThru->setGeometry(QRect(210, 40, 87, 20));
        pbStartCond = new QPushButton(centralwidget);
        pbStartCond->setObjectName(QString::fromUtf8("pbStartCond"));
        pbStartCond->setGeometry(QRect(480, 420, 131, 32));
        pbStopCond = new QPushButton(centralwidget);
        pbStopCond->setObjectName(QString::fromUtf8("pbStopCond"));
        pbStopCond->setGeometry(QRect(480, 450, 131, 32));
        lvMessages = new QListWidget(centralwidget);
        lvMessages->setObjectName(QString::fromUtf8("lvMessages"));
        lvMessages->setGeometry(QRect(20, 150, 441, 201));
        pbMatrixMode = new QPushButton(centralwidget);
        pbMatrixMode->setObjectName(QString::fromUtf8("pbMatrixMode"));
        pbMatrixMode->setGeometry(QRect(480, 480, 131, 32));
        pbRawMode = new QPushButton(centralwidget);
        pbRawMode->setObjectName(QString::fromUtf8("pbRawMode"));
        pbRawMode->setGeometry(QRect(480, 510, 131, 32));
        pbKill = new QPushButton(centralwidget);
        pbKill->setObjectName(QString::fromUtf8("pbKill"));
        pbKill->setGeometry(QRect(410, 85, 101, 32));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuRadio_Baton = new QMenu(menubar);
        menuRadio_Baton->setObjectName(QString::fromUtf8("menuRadio_Baton"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuRadio_Baton->menuAction());
        menuRadio_Baton->addAction(actionNew_Configuration);
        menuRadio_Baton->addAction(actionOpen_Configuration);
        menuRadio_Baton->addSeparator();
        menuRadio_Baton->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Radio Baton", 0, QApplication::UnicodeUTF8));
        actionNew_Configuration->setText(QApplication::translate("MainWindow", "New Configuration", 0, QApplication::UnicodeUTF8));
        actionOpen_Configuration->setText(QApplication::translate("MainWindow", "Open Configuration", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        pbEditFile->setText(QApplication::translate("MainWindow", "Edit File", 0, QApplication::UnicodeUTF8));
        cbNote->setText(QApplication::translate("MainWindow", "Note", 0, QApplication::UnicodeUTF8));
        cbPolyAftertouch->setText(QApplication::translate("MainWindow", "Poly Aftertouch", 0, QApplication::UnicodeUTF8));
        cbControllerChange->setText(QApplication::translate("MainWindow", "Controller Change", 0, QApplication::UnicodeUTF8));
        cbProgramChange->setText(QApplication::translate("MainWindow", "Program Change", 0, QApplication::UnicodeUTF8));
        cbMonoAftertouch->setText(QApplication::translate("MainWindow", "Mono Aftertouch", 0, QApplication::UnicodeUTF8));
        cbPitchBend->setText(QApplication::translate("MainWindow", "Pitch Bend", 0, QApplication::UnicodeUTF8));
        lConductDisplay->setText(QApplication::translate("MainWindow", "Conduct Display", 0, QApplication::UnicodeUTF8));
        lMode->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        cbMode->clear();
        cbMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Conduct", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Runs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Rhythmic", 0, QApplication::UnicodeUTF8)
        );
        lMidiIn->setText(QApplication::translate("MainWindow", "MIDI In:", 0, QApplication::UnicodeUTF8));
        lMidiOut->setText(QApplication::translate("MainWindow", "MIDI Out:", 0, QApplication::UnicodeUTF8));
        lMessages->setText(QApplication::translate("MainWindow", "Messages", 0, QApplication::UnicodeUTF8));
        cbDisplayMidi->setText(QApplication::translate("MainWindow", "Display MIDI messages", 0, QApplication::UnicodeUTF8));
        lFilters->setText(QApplication::translate("MainWindow", "Filters", 0, QApplication::UnicodeUTF8));
        pbCompile->setText(QApplication::translate("MainWindow", "Compile", 0, QApplication::UnicodeUTF8));
        cbSysEx->setText(QApplication::translate("MainWindow", "SysEx", 0, QApplication::UnicodeUTF8));
        cbRealTime->setText(QApplication::translate("MainWindow", "Real Time", 0, QApplication::UnicodeUTF8));
        pbUpdateDevices->setText(QApplication::translate("MainWindow", "Update Devices", 0, QApplication::UnicodeUTF8));
        lConduct->setText(QApplication::translate("MainWindow", "Conductor File:", 0, QApplication::UnicodeUTF8));
        pbConductOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        lConductFile->setText(QApplication::translate("MainWindow", "None", 0, QApplication::UnicodeUTF8));
        cbThru->setText(QApplication::translate("MainWindow", "Thru", 0, QApplication::UnicodeUTF8));
        pbStartCond->setText(QApplication::translate("MainWindow", "Start Conducting", 0, QApplication::UnicodeUTF8));
        pbStopCond->setText(QApplication::translate("MainWindow", "Stop Conducting", 0, QApplication::UnicodeUTF8));
        pbMatrixMode->setText(QApplication::translate("MainWindow", "Matrix Mode", 0, QApplication::UnicodeUTF8));
        pbRawMode->setText(QApplication::translate("MainWindow", "Raw Mode", 0, QApplication::UnicodeUTF8));
        pbKill->setText(QApplication::translate("MainWindow", "Kill", 0, QApplication::UnicodeUTF8));
        menuRadio_Baton->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
