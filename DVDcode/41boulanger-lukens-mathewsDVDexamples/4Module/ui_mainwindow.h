/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Jul 8 14:50:41 2010
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pbSinReset;
    QSlider *vsPitch;
    QSlider *vsAttack;
    QSlider *vsDecay;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *lbPitch;
    QLabel *lbAttack;
    QLabel *lbDecay;
    QComboBox *cbWave;
    QSlider *vsTime;
    QLabel *label_4;
    QLabel *lbAutoTime;
    QLabel *lbAutoTime_2;
    QCheckBox *cbAutoPlay;
    QLabel *label_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 319);
        MainWindow->setMinimumSize(QSize(400, 319));
        MainWindow->setMaximumSize(QSize(400, 319));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pbSinReset = new QPushButton(centralWidget);
        pbSinReset->setObjectName(QString::fromUtf8("pbSinReset"));
        pbSinReset->setGeometry(QRect(50, 230, 113, 32));
        vsPitch = new QSlider(centralWidget);
        vsPitch->setObjectName(QString::fromUtf8("vsPitch"));
        vsPitch->setGeometry(QRect(20, 40, 22, 160));
        vsPitch->setMinimum(20);
        vsPitch->setMaximum(1000);
        vsPitch->setOrientation(Qt::Vertical);
        vsAttack = new QSlider(centralWidget);
        vsAttack->setObjectName(QString::fromUtf8("vsAttack"));
        vsAttack->setGeometry(QRect(130, 40, 22, 160));
        vsAttack->setMinimum(1);
        vsAttack->setMaximum(500);
        vsAttack->setOrientation(Qt::Vertical);
        vsDecay = new QSlider(centralWidget);
        vsDecay->setObjectName(QString::fromUtf8("vsDecay"));
        vsDecay->setGeometry(QRect(180, 40, 22, 160));
        vsDecay->setMinimum(1);
        vsDecay->setMaximum(500);
        vsDecay->setOrientation(Qt::Vertical);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 61, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(120, 20, 61, 16));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(170, 20, 61, 16));
        lbPitch = new QLabel(centralWidget);
        lbPitch->setObjectName(QString::fromUtf8("lbPitch"));
        lbPitch->setGeometry(QRect(20, 200, 61, 16));
        lbAttack = new QLabel(centralWidget);
        lbAttack->setObjectName(QString::fromUtf8("lbAttack"));
        lbAttack->setGeometry(QRect(130, 200, 61, 16));
        lbDecay = new QLabel(centralWidget);
        lbDecay->setObjectName(QString::fromUtf8("lbDecay"));
        lbDecay->setGeometry(QRect(180, 200, 61, 16));
        cbWave = new QComboBox(centralWidget);
        cbWave->setObjectName(QString::fromUtf8("cbWave"));
        cbWave->setGeometry(QRect(50, 110, 81, 26));
        vsTime = new QSlider(centralWidget);
        vsTime->setObjectName(QString::fromUtf8("vsTime"));
        vsTime->setGeometry(QRect(310, 40, 22, 160));
        vsTime->setMinimum(1);
        vsTime->setMaximum(3000);
        vsTime->setSliderPosition(1000);
        vsTime->setOrientation(Qt::Vertical);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(290, 20, 81, 16));
        lbAutoTime = new QLabel(centralWidget);
        lbAutoTime->setObjectName(QString::fromUtf8("lbAutoTime"));
        lbAutoTime->setGeometry(QRect(290, 200, 41, 16));
        lbAutoTime_2 = new QLabel(centralWidget);
        lbAutoTime_2->setObjectName(QString::fromUtf8("lbAutoTime_2"));
        lbAutoTime_2->setGeometry(QRect(330, 200, 41, 16));
        cbAutoPlay = new QCheckBox(centralWidget);
        cbAutoPlay->setObjectName(QString::fromUtf8("cbAutoPlay"));
        cbAutoPlay->setGeometry(QRect(280, 230, 87, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 90, 81, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Module", 0, QApplication::UnicodeUTF8));
        pbSinReset->setText(QApplication::translate("MainWindow", "Play Note", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Pitch", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Attack", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Decay", 0, QApplication::UnicodeUTF8));
        lbPitch->setText(QApplication::translate("MainWindow", "440", 0, QApplication::UnicodeUTF8));
        lbAttack->setText(QApplication::translate("MainWindow", ".10", 0, QApplication::UnicodeUTF8));
        lbDecay->setText(QApplication::translate("MainWindow", ".20", 0, QApplication::UnicodeUTF8));
        cbWave->clear();
        cbWave->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Sine", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Square", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Saw", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Tri", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("MainWindow", "Auto Time", 0, QApplication::UnicodeUTF8));
        lbAutoTime->setText(QApplication::translate("MainWindow", "1000", 0, QApplication::UnicodeUTF8));
        lbAutoTime_2->setText(QApplication::translate("MainWindow", "ms", 0, QApplication::UnicodeUTF8));
        cbAutoPlay->setText(QApplication::translate("MainWindow", "Auto Play", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Wave Shape", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
