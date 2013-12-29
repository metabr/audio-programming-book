/********************************************************************************
** Form generated from reading UI file 'matrixmode.ui'
**
** Created: Wed Jun 23 14:58:39 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATRIXMODE_H
#define UI_MATRIXMODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatrixMode
{
public:
    QPushButton *pbClose;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_9;
    QLineEdit *leValue1;
    QLineEdit *leValue2;
    QLineEdit *leValue3;
    QLineEdit *leValue4;
    QLineEdit *leValue5;
    QLineEdit *leValue6;
    QLineEdit *leValue7;
    QLabel *label_10;
    QLineEdit *leScale1;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *leScale2;
    QLineEdit *leScale3;
    QLineEdit *leScale4;
    QLineEdit *leScale5;
    QLineEdit *leScale6;
    QLineEdit *leScale7;
    QLineEdit *leOffset1;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *leOffset2;
    QLineEdit *leOffset3;
    QLineEdit *leOffset4;
    QLineEdit *leOffset5;
    QLineEdit *leOffset6;
    QLineEdit *leOffset7;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLineEdit *leOutput1;
    QLineEdit *leOutput2;
    QLineEdit *leOutput3;
    QLineEdit *leOutput4;
    QLineEdit *leOutput5;
    QLineEdit *leOutput6;
    QLineEdit *leOutput7;
    QLabel *label_31;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QComboBox *cbOut1;
    QLabel *label_6;
    QComboBox *cbOut2;
    QLabel *label_7;
    QComboBox *cbOut3;
    QLabel *label_8;
    QComboBox *cbOut4;
    QLabel *label_32;
    QComboBox *cbOut5;
    QLabel *label_33;
    QComboBox *cbOut6;
    QLabel *label_34;
    QComboBox *cbOut7;

    void setupUi(QDialog *MatrixMode)
    {
        if (MatrixMode->objectName().isEmpty())
            MatrixMode->setObjectName(QString::fromUtf8("MatrixMode"));
        MatrixMode->resize(702, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MatrixMode->sizePolicy().hasHeightForWidth());
        MatrixMode->setSizePolicy(sizePolicy);
        MatrixMode->setMinimumSize(QSize(702, 300));
        MatrixMode->setMaximumSize(QSize(702, 300));
        MatrixMode->setFocusPolicy(Qt::NoFocus);
        pbClose = new QPushButton(MatrixMode);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setGeometry(QRect(590, 270, 113, 32));
        pbClose->setFocusPolicy(Qt::NoFocus);
        label = new QLabel(MatrixMode);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 61, 16));
        label_2 = new QLabel(MatrixMode);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(160, 20, 61, 16));
        label_3 = new QLabel(MatrixMode);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(430, 20, 61, 16));
        label_4 = new QLabel(MatrixMode);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(520, 20, 61, 16));
        label_9 = new QLabel(MatrixMode);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(340, 20, 61, 16));
        leValue1 = new QLineEdit(MatrixMode);
        leValue1->setObjectName(QString::fromUtf8("leValue1"));
        leValue1->setGeometry(QRect(340, 50, 41, 22));
        leValue2 = new QLineEdit(MatrixMode);
        leValue2->setObjectName(QString::fromUtf8("leValue2"));
        leValue2->setGeometry(QRect(340, 80, 41, 22));
        leValue3 = new QLineEdit(MatrixMode);
        leValue3->setObjectName(QString::fromUtf8("leValue3"));
        leValue3->setGeometry(QRect(340, 110, 41, 22));
        leValue4 = new QLineEdit(MatrixMode);
        leValue4->setObjectName(QString::fromUtf8("leValue4"));
        leValue4->setGeometry(QRect(340, 140, 41, 22));
        leValue5 = new QLineEdit(MatrixMode);
        leValue5->setObjectName(QString::fromUtf8("leValue5"));
        leValue5->setGeometry(QRect(340, 170, 41, 22));
        leValue6 = new QLineEdit(MatrixMode);
        leValue6->setObjectName(QString::fromUtf8("leValue6"));
        leValue6->setGeometry(QRect(340, 200, 41, 22));
        leValue7 = new QLineEdit(MatrixMode);
        leValue7->setObjectName(QString::fromUtf8("leValue7"));
        leValue7->setGeometry(QRect(340, 230, 41, 22));
        label_10 = new QLabel(MatrixMode);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(400, 50, 16, 16));
        leScale1 = new QLineEdit(MatrixMode);
        leScale1->setObjectName(QString::fromUtf8("leScale1"));
        leScale1->setGeometry(QRect(430, 50, 41, 22));
        label_11 = new QLabel(MatrixMode);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(400, 80, 16, 16));
        label_12 = new QLabel(MatrixMode);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(400, 110, 16, 16));
        label_13 = new QLabel(MatrixMode);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(400, 140, 16, 16));
        label_14 = new QLabel(MatrixMode);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(400, 170, 16, 16));
        label_15 = new QLabel(MatrixMode);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(400, 200, 16, 16));
        label_16 = new QLabel(MatrixMode);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(400, 230, 16, 16));
        leScale2 = new QLineEdit(MatrixMode);
        leScale2->setObjectName(QString::fromUtf8("leScale2"));
        leScale2->setGeometry(QRect(430, 80, 41, 22));
        leScale3 = new QLineEdit(MatrixMode);
        leScale3->setObjectName(QString::fromUtf8("leScale3"));
        leScale3->setGeometry(QRect(430, 110, 41, 22));
        leScale4 = new QLineEdit(MatrixMode);
        leScale4->setObjectName(QString::fromUtf8("leScale4"));
        leScale4->setGeometry(QRect(430, 140, 41, 22));
        leScale5 = new QLineEdit(MatrixMode);
        leScale5->setObjectName(QString::fromUtf8("leScale5"));
        leScale5->setGeometry(QRect(430, 170, 41, 22));
        leScale6 = new QLineEdit(MatrixMode);
        leScale6->setObjectName(QString::fromUtf8("leScale6"));
        leScale6->setGeometry(QRect(430, 200, 41, 22));
        leScale7 = new QLineEdit(MatrixMode);
        leScale7->setObjectName(QString::fromUtf8("leScale7"));
        leScale7->setGeometry(QRect(430, 230, 41, 22));
        leOffset1 = new QLineEdit(MatrixMode);
        leOffset1->setObjectName(QString::fromUtf8("leOffset1"));
        leOffset1->setGeometry(QRect(520, 50, 41, 22));
        label_17 = new QLabel(MatrixMode);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(490, 50, 16, 16));
        label_18 = new QLabel(MatrixMode);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(490, 80, 16, 16));
        leOffset2 = new QLineEdit(MatrixMode);
        leOffset2->setObjectName(QString::fromUtf8("leOffset2"));
        leOffset2->setGeometry(QRect(520, 80, 41, 22));
        leOffset3 = new QLineEdit(MatrixMode);
        leOffset3->setObjectName(QString::fromUtf8("leOffset3"));
        leOffset3->setGeometry(QRect(520, 110, 41, 22));
        leOffset4 = new QLineEdit(MatrixMode);
        leOffset4->setObjectName(QString::fromUtf8("leOffset4"));
        leOffset4->setGeometry(QRect(520, 140, 41, 22));
        leOffset5 = new QLineEdit(MatrixMode);
        leOffset5->setObjectName(QString::fromUtf8("leOffset5"));
        leOffset5->setGeometry(QRect(520, 170, 41, 22));
        leOffset6 = new QLineEdit(MatrixMode);
        leOffset6->setObjectName(QString::fromUtf8("leOffset6"));
        leOffset6->setGeometry(QRect(520, 200, 41, 22));
        leOffset7 = new QLineEdit(MatrixMode);
        leOffset7->setObjectName(QString::fromUtf8("leOffset7"));
        leOffset7->setGeometry(QRect(520, 230, 41, 22));
        label_19 = new QLabel(MatrixMode);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(490, 110, 16, 16));
        label_20 = new QLabel(MatrixMode);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(490, 140, 16, 16));
        label_21 = new QLabel(MatrixMode);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(490, 170, 16, 16));
        label_22 = new QLabel(MatrixMode);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(490, 200, 16, 16));
        label_23 = new QLabel(MatrixMode);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(490, 230, 16, 16));
        label_24 = new QLabel(MatrixMode);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(580, 50, 16, 16));
        label_25 = new QLabel(MatrixMode);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(580, 80, 16, 16));
        label_26 = new QLabel(MatrixMode);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(580, 110, 16, 16));
        label_27 = new QLabel(MatrixMode);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(580, 140, 16, 16));
        label_28 = new QLabel(MatrixMode);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(580, 170, 16, 16));
        label_29 = new QLabel(MatrixMode);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(580, 200, 16, 16));
        label_30 = new QLabel(MatrixMode);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(580, 230, 16, 16));
        leOutput1 = new QLineEdit(MatrixMode);
        leOutput1->setObjectName(QString::fromUtf8("leOutput1"));
        leOutput1->setGeometry(QRect(610, 50, 71, 22));
        leOutput2 = new QLineEdit(MatrixMode);
        leOutput2->setObjectName(QString::fromUtf8("leOutput2"));
        leOutput2->setGeometry(QRect(610, 80, 71, 22));
        leOutput3 = new QLineEdit(MatrixMode);
        leOutput3->setObjectName(QString::fromUtf8("leOutput3"));
        leOutput3->setGeometry(QRect(610, 110, 71, 22));
        leOutput4 = new QLineEdit(MatrixMode);
        leOutput4->setObjectName(QString::fromUtf8("leOutput4"));
        leOutput4->setGeometry(QRect(610, 140, 71, 22));
        leOutput5 = new QLineEdit(MatrixMode);
        leOutput5->setObjectName(QString::fromUtf8("leOutput5"));
        leOutput5->setGeometry(QRect(610, 170, 71, 22));
        leOutput6 = new QLineEdit(MatrixMode);
        leOutput6->setObjectName(QString::fromUtf8("leOutput6"));
        leOutput6->setGeometry(QRect(610, 200, 71, 22));
        leOutput7 = new QLineEdit(MatrixMode);
        leOutput7->setObjectName(QString::fromUtf8("leOutput7"));
        leOutput7->setGeometry(QRect(610, 230, 71, 22));
        label_31 = new QLabel(MatrixMode);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(610, 20, 61, 16));
        layoutWidget = new QWidget(MatrixMode);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 50, 291, 206));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        cbOut1 = new QComboBox(layoutWidget);
        cbOut1->setObjectName(QString::fromUtf8("cbOut1"));

        gridLayout->addWidget(cbOut1, 0, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        cbOut2 = new QComboBox(layoutWidget);
        cbOut2->setObjectName(QString::fromUtf8("cbOut2"));

        gridLayout->addWidget(cbOut2, 1, 1, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        cbOut3 = new QComboBox(layoutWidget);
        cbOut3->setObjectName(QString::fromUtf8("cbOut3"));

        gridLayout->addWidget(cbOut3, 2, 1, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 0, 1, 1);

        cbOut4 = new QComboBox(layoutWidget);
        cbOut4->setObjectName(QString::fromUtf8("cbOut4"));

        gridLayout->addWidget(cbOut4, 3, 1, 1, 1);

        label_32 = new QLabel(layoutWidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout->addWidget(label_32, 4, 0, 1, 1);

        cbOut5 = new QComboBox(layoutWidget);
        cbOut5->setObjectName(QString::fromUtf8("cbOut5"));

        gridLayout->addWidget(cbOut5, 4, 1, 1, 1);

        label_33 = new QLabel(layoutWidget);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout->addWidget(label_33, 5, 0, 1, 1);

        cbOut6 = new QComboBox(layoutWidget);
        cbOut6->setObjectName(QString::fromUtf8("cbOut6"));

        gridLayout->addWidget(cbOut6, 5, 1, 1, 1);

        label_34 = new QLabel(layoutWidget);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout->addWidget(label_34, 6, 0, 1, 1);

        cbOut7 = new QComboBox(layoutWidget);
        cbOut7->setObjectName(QString::fromUtf8("cbOut7"));

        gridLayout->addWidget(cbOut7, 6, 1, 1, 1);


        retranslateUi(MatrixMode);

        QMetaObject::connectSlotsByName(MatrixMode);
    } // setupUi

    void retranslateUi(QDialog *MatrixMode)
    {
        MatrixMode->setWindowTitle(QApplication::translate("MatrixMode", "Dialog", 0, QApplication::UnicodeUTF8));
        pbClose->setText(QApplication::translate("MatrixMode", "Close", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MatrixMode", "MIDI In", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MatrixMode", "MIDI Out", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MatrixMode", "Scale", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MatrixMode", "Offset", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MatrixMode", "Value", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MatrixMode", "x", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MatrixMode", "+", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MatrixMode", "=", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MatrixMode", "Output", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MatrixMode", "Note Off", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MatrixMode", "Note On", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MatrixMode", "Poly Aftertouch", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MatrixMode", "Control Change", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MatrixMode", "Program Change", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MatrixMode", "Channel Pressure", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MatrixMode", "Pitch Bend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MatrixMode: public Ui_MatrixMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATRIXMODE_H
