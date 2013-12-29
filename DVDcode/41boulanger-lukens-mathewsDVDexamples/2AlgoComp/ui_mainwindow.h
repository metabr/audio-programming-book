/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Jul 8 22:53:05 2010
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
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QComboBox *cbMode;
    QComboBox *cbKey;
    QComboBox *cbMelodyOctave;
    QComboBox *cbBassOffset;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *leTempo;
    QLineEdit *leRestChance;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *leNumMeasures;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leFileName;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QComboBox *cbVariation;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_10;
    QComboBox *cbDrums;
    QPushButton *pbGenerate;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_11;
    QComboBox *cbWriteTo;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        cbMode = new QComboBox(centralWidget);
        cbMode->setObjectName(QString::fromUtf8("cbMode"));
        cbMode->setGeometry(QRect(10, 80, 111, 26));
        cbKey = new QComboBox(centralWidget);
        cbKey->setObjectName(QString::fromUtf8("cbKey"));
        cbKey->setGeometry(QRect(160, 80, 111, 26));
        cbMelodyOctave = new QComboBox(centralWidget);
        cbMelodyOctave->setObjectName(QString::fromUtf8("cbMelodyOctave"));
        cbMelodyOctave->setGeometry(QRect(310, 80, 111, 26));
        cbBassOffset = new QComboBox(centralWidget);
        cbBassOffset->setObjectName(QString::fromUtf8("cbBassOffset"));
        cbBassOffset->setGeometry(QRect(460, 80, 111, 26));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 60, 62, 22));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(200, 60, 62, 22));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(320, 60, 101, 22));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(440, 60, 161, 22));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 140, 51, 22));
        leTempo = new QLineEdit(centralWidget);
        leTempo->setObjectName(QString::fromUtf8("leTempo"));
        leTempo->setGeometry(QRect(90, 140, 61, 22));
        leTempo->setLayoutDirection(Qt::LeftToRight);
        leRestChance = new QLineEdit(centralWidget);
        leRestChance->setObjectName(QString::fromUtf8("leRestChance"));
        leRestChance->setGeometry(QRect(140, 210, 31, 22));
        leRestChance->setLayoutDirection(Qt::LeftToRight);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 210, 101, 22));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(190, 210, 171, 22));
        leNumMeasures = new QLineEdit(centralWidget);
        leNumMeasures->setObjectName(QString::fromUtf8("leNumMeasures"));
        leNumMeasures->setGeometry(QRect(360, 210, 41, 22));
        leNumMeasures->setLayoutDirection(Qt::LeftToRight);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 10, 216, 24));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        leFileName = new QLineEdit(layoutWidget);
        leFileName->setObjectName(QString::fromUtf8("leFileName"));

        horizontalLayout->addWidget(leFileName);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(190, 140, 333, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_2->addWidget(label_7);

        cbVariation = new QComboBox(layoutWidget1);
        cbVariation->setObjectName(QString::fromUtf8("cbVariation"));

        horizontalLayout_2->addWidget(cbVariation);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(420, 210, 128, 26));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(layoutWidget2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_3->addWidget(label_10);

        cbDrums = new QComboBox(layoutWidget2);
        cbDrums->setObjectName(QString::fromUtf8("cbDrums"));

        horizontalLayout_3->addWidget(cbDrums);

        pbGenerate = new QPushButton(centralWidget);
        pbGenerate->setObjectName(QString::fromUtf8("pbGenerate"));
        pbGenerate->setGeometry(QRect(430, 310, 97, 32));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(220, 270, 141, 26));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(widget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_4->addWidget(label_11);

        cbWriteTo = new QComboBox(widget);
        cbWriteTo->setObjectName(QString::fromUtf8("cbWriteTo"));

        horizontalLayout_4->addWidget(cbWriteTo);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        cbMode->clear();
        cbMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Ionian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Dorian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Locrian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Lydian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Mixolydian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Aeolian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Phrygian", 0, QApplication::UnicodeUTF8)
        );
        cbKey->clear();
        cbKey->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "A#/Bb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "C", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "C#/Db", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "D", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "D#/Eb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "E", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "F", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "F#/Gb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "G", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "G#/Ab", 0, QApplication::UnicodeUTF8)
        );
        cbMelodyOctave->clear();
        cbMelodyOctave->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "8", 0, QApplication::UnicodeUTF8)
        );
        cbBassOffset->clear();
        cbBassOffset->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Key", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Melody Octave", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Bass Offset from Melody", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Tempo", 0, QApplication::UnicodeUTF8));
        leTempo->setText(QApplication::translate("MainWindow", "120", 0, QApplication::UnicodeUTF8));
        leRestChance->setText(QApplication::translate("MainWindow", "25", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Rest Chance %", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Number of Measures (4/4)", 0, QApplication::UnicodeUTF8));
        leNumMeasures->setText(QApplication::translate("MainWindow", "16", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "FileName:", 0, QApplication::UnicodeUTF8));
        leFileName->setText(QApplication::translate("MainWindow", "default.csd", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Variations", 0, QApplication::UnicodeUTF8));
        cbVariation->clear();
        cbVariation->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Quarter Notes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Quarters & Eighths", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Quarters, Eighths, and Sixteenths", 0, QApplication::UnicodeUTF8)
        );
        label_10->setText(QApplication::translate("MainWindow", "Drums?", 0, QApplication::UnicodeUTF8));
        cbDrums->clear();
        cbDrums->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Yes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "No", 0, QApplication::UnicodeUTF8)
        );
        pbGenerate->setText(QApplication::translate("MainWindow", "Generate", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Write to:", 0, QApplication::UnicodeUTF8));
        cbWriteTo->clear();
        cbWriteTo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "DAC", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
