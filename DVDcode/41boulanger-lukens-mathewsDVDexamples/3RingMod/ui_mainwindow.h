/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Jul 10 17:20:23 2010
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QSlider *vsFreq;
    QComboBox *cbWaveShape;
    QLabel *label;
    QLabel *label_2;
    QLabel *lbFreq;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(242, 277);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        vsFreq = new QSlider(centralWidget);
        vsFreq->setObjectName(QString::fromUtf8("vsFreq"));
        vsFreq->setGeometry(QRect(30, 40, 22, 160));
        vsFreq->setMinimum(1);
        vsFreq->setMaximum(1000);
        vsFreq->setValue(40);
        vsFreq->setOrientation(Qt::Vertical);
        cbWaveShape = new QComboBox(centralWidget);
        cbWaveShape->setObjectName(QString::fromUtf8("cbWaveShape"));
        cbWaveShape->setGeometry(QRect(110, 40, 111, 26));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 71, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 10, 81, 16));
        lbFreq = new QLabel(centralWidget);
        lbFreq->setObjectName(QString::fromUtf8("lbFreq"));
        lbFreq->setGeometry(QRect(30, 200, 41, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 242, 22));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Ring Mod", 0, QApplication::UnicodeUTF8));
        cbWaveShape->clear();
        cbWaveShape->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Sine", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Saw", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Triangle", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("MainWindow", "Frequency", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Wave Shape", 0, QApplication::UnicodeUTF8));
        lbFreq->setText(QApplication::translate("MainWindow", "40", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
