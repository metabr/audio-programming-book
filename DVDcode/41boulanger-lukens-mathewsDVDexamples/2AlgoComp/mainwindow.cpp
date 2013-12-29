#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <QByteArray>
#include <QMessageBox>

int modal[7][8] = { {0, 2, 4, 5, 7, 9, 11, 12}, {0, 2, 3, 5, 7, 9, 10, 12}, {0, 1, 3, 5, 7, 8, 10, 12}, {0, 2, 4, 6, 7, 9, 11, 12}, {0, 2, 4, 5, 7, 9, 10, 12}, {0, 2, 3, 5, 7, 8, 10, 12}, {0, 1, 3, 5, 6, 8, 10, 12} };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Do all initialization
    init();
}

void MainWindow::init()
{
    //connect the generate button to its function
    connect(ui->pbGenerate, SIGNAL(clicked()), this, SLOT(generate()));
}

void MainWindow::generate()
{
    //write instruments returns 0 if everything is good, otherwise it returns 1
    if(!writeInstruments())
    {
        writeScore();
        writeDrums();
    }
}

int MainWindow::writeInstruments()
{
    //set local variables from UI
    const char *f;
    f = (char*)malloc(sizeof(char)*128);
    QByteArray ba = ui->leFileName->text().toLatin1();

    f = ba;

    FILE* fp=fopen(f, "w+");
    if (fp == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Error opening file. Perhaps you are running this from a write protected folder.");
        msgBox.exec();
        return 1;
    }
    fprintf(fp, "<CsoundSynthesizer>\n");
    fprintf(fp, "<CsInstruments>\n\n");
    fprintf(fp, "\tinstr 1\n");
    fprintf(fp, "idur\t\t=\tp3\n");
    fprintf(fp, "iamp\t\t=\tp4\n");
    fprintf(fp, "ifreq\t\t=	cpsmidinn(p5)\n");
    fprintf(fp, "ifun\t\t=\tp6\n\n");

    fprintf(fp, "k1	linen\t\t1, .05, p3, .05\n");
    fprintf(fp, "kpenv\tlinseg\t0, idur/20, 100, idur/20, 0\n");
    fprintf(fp, "kcf\texpseg\t20, idur/4, 4000, idur/4 * 3, 4000.1\n");
    fprintf(fp, "krand\trand\t\t200\n");
    fprintf(fp, "a1\toscil\t\tiamp, ifreq + kpenv, ifun\n");
    fprintf(fp, "a2\toscil\t\tiamp, ifreq * 1.01 + kpenv, ifun\n");
    fprintf(fp, "a3\toscil\t\tiamp, ifreq * .99 + kpenv, ifun\n");
    fprintf(fp, "afilt\tbutbp\t\t(a1+a2+a3)/3, kcf, 8000\n");
    fprintf(fp, "\tout\t\tafilt * k1\n");
    fprintf(fp, "\tendin\n\n");

    fprintf(fp, "\tinstr 2\n");
    fprintf(fp, "idur\t\t=\tp3\n");
    fprintf(fp, "iamp\t\t=\tp4\n");
    fprintf(fp, "ifreq\t\t=	cpsmidinn(p5)\n");
    fprintf(fp, "ifun\t\t=\tp6\n\n");

    fprintf(fp, "k1	linen\t\t1, .05, p3, .05\n");
    fprintf(fp, "kpenv\tlinseg\t0, idur/50, 100, idur/50, 0\n");
    fprintf(fp, "kcf\texpseg\t20, idur/4, 1000, idur/4 * 3, 1000.1\n");
    fprintf(fp, "krand\trand\t\t200\n");
    fprintf(fp, "a1\toscil\t\tiamp, ifreq + kpenv, ifun\n");
    fprintf(fp, "a2\toscil\t\tiamp, ifreq * 1.01 + kpenv, ifun\n");
    fprintf(fp, "a3\toscil\t\tiamp, ifreq * .99 + kpenv, ifun\n");
    fprintf(fp, "afilt\tbutbp\t\t(a1+a2+a3)/3, kcf, 8000\n");
    fprintf(fp, "\tout\t\tafilt * k1\n");
    fprintf(fp, "\tendin\n");

    fprintf(fp, "\tinstr 3; hihat\n");

    fprintf(fp, "ilen init p3\n");
    fprintf(fp, "iamp init p4\n\n");

    fprintf(fp, "kcutfreq\texpon\t\t10000, 0.1, 2500\n");
    fprintf(fp, "aamp\t\texpon\t\tiamp, 0.1, 10\n");
    fprintf(fp, "arand\t\trand\t\taamp\n");
    fprintf(fp, "alp1\t\tbutterlp\tarand, kcutfreq\n");
    fprintf(fp, "alp2\t\tbutterlp\talp1, kcutfreq\n");
    fprintf(fp, "ahp1\t\tbutterhp\talp2, 3500\n");
    fprintf(fp, "asigpre\tbutterhp\tahp1, 3500\n");
    fprintf(fp, "asig\t\tlinen\t\t(asigpre+arand/2), 0, ilen, .05\n\n");

    fprintf(fp, "out\t\tasig\n");
    fprintf(fp, "\tendin\n\n");

    fprintf(fp, "\tinstr 4; snare\n");
    fprintf(fp, "icps0\t= 147\n");
    fprintf(fp, "iamp\t= p4*0.7\n\n");

    fprintf(fp, "icps1\t=  2.0 * icps0\n\n");

    fprintf(fp, "kcps\tport icps0, 0.007, icps1\n");
    fprintf(fp, "kcpsx\t= kcps * 1.5\n\n");

    fprintf(fp, "kfmd\tport\t0.0, 0.01, 0.7\n");
    fprintf(fp, "aenv1\texpon\t1.0, 0.03, 0.5\n");
    fprintf(fp, "kenv2\tport\t1.0, 0.008, 0.0\n");
    fprintf(fp, "aenv2\tinterp kenv2\n");
    fprintf(fp, "aenv3\texpon\t1.0, 0.025, 0.5\n\n");

    fprintf(fp, "a_\toscili 1.0, kcps, 1\n");
    fprintf(fp, "a1\toscili 1.0, kcps * (1.0 + a_*kfmd), 1\n");
    fprintf(fp, "a_\toscili 1.0, kcpsx, 1\n");
    fprintf(fp, "a2\toscili 1.0, kcpsx * (1.0 + a_*kfmd), 1\n\n");

    fprintf(fp, "a3\tunirand 2.0\n");
    fprintf(fp, "a3\t=  a3 - 1.0\n");
    fprintf(fp, "a3\tbutterbp a3, 5000, 7500\n");
    fprintf(fp, "a3\t= a3 * aenv2\n\n");

    fprintf(fp, "a0\t= a1 + a2*aenv3 + a3*1.0\n");
    fprintf(fp, "a0\t= a0 * aenv1\n\n");

    fprintf(fp, "outs a0*iamp\n");
    fprintf(fp, "\tendin\n\n");

    fprintf(fp, "\tinstr 5; kick\n");
    fprintf(fp, "iamp\t= p4\n\n");

    fprintf(fp, "k1\texpon\t\t120, .2, 50\n");
    fprintf(fp, "k2\texpon\t\t500, .4, 200\n");
    fprintf(fp, "a1\toscil\t\tiamp, k1, 1\n");
    fprintf(fp, "a2\treson\t\ta1, k2, 50\n");
    fprintf(fp, "a3\tbutterlp\ta2+a1, k1, 1\n");
    fprintf(fp, "a4\tbutterlp\ta3, k1, 1\n");
    fprintf(fp, "a5\tbutterlp\ta4, 2500, 1\n");
    fprintf(fp, "a6\tbutterhp\ta5, 50\n");
    fprintf(fp, "a7\tbutterhp\ta6, 50\n");
    fprintf(fp, "a8\tlinen\t\ta7, 0.01, p3, .2\n\n");

    fprintf(fp, "outs a8\n");
    fprintf(fp, "\tendin\n\n");

    fprintf(fp, "</CsInstruments>\n");
    fprintf(fp, "<CsScore>\n");
    fprintf(fp, "f 1 0 4096 10 1 .5 .25 .125 .0625 1\n\n");

    fclose(fp);
    return 0;
}

void MainWindow::writeScore()
{
    //Set all of the local variables from the UI
    float tempo = ui->leTempo->text().toFloat();
    int mode = ui->cbMode->currentIndex() + 1;
    int scale = ui->cbKey->currentIndex() - 3;
    if(scale < 0) scale += 10;
    int octave = ui->cbMelodyOctave->currentIndex() + 1;
    int bass = ui->cbBassOffset->currentIndex() + 1;
    bass *= 12;
    int tvariation = ui->cbVariation->currentIndex() + 1;
    int rests = ui->leRestChance->text().toInt();
    int length = ui->leNumMeasures->text().toInt();

    const char *f;
    f = (char*)malloc(sizeof(char)*128);
    QByteArray ba = ui->leFileName->text().toLatin1();

    f = ba;

    int rtemp, rtemp2, randrest, note, note2, vol;
    float nleng, loop, step, time = 0;
    FILE* fp=fopen(f, "a");

    for (loop = 0; loop < length; loop += step)
    {
        rtemp = (rand() % tvariation) + 1;
        if (rtemp == 1 && (loop - (int)loop) >= .8125) rtemp = 2;	//if less than a quarter note left in measure
        if (rtemp == 2 && (loop - (int)loop) >= .9375) rtemp = 3;	//if less than an eighth note left in measure
        if (loop - (int)loop == 0) rtemp = 1;	//new measure starts with a quarter note
        switch (rtemp)
        {
            case 1: nleng = 60 / tempo; step = .25; break;
            case 2: nleng = 60 / tempo / 2; step = .125; break;
            case 3: nleng = 60 / tempo / 4; step = .0625; break;
        }

        rtemp = (rand() % 8) + 1;
        if (loop == 0) rtemp = 1;	//start on tonic
        note = 12 * (octave + 1) + scale + modal[mode - 1][rtemp - 1];

        randrest = (rand() % 100) + 1;
        if (randrest <= rests)
            vol = 0;
        else
            vol = 8000;

        fprintf(fp, "i 1 %f %f %d %d 1\n", time, nleng, vol, note);

        if (loop - (int)loop == 0) //new measure = new bass note
        {
            rtemp2 = (rand() % 3);
            if (loop == 0) note2 = note - bass;	//start on tonic

            note2 = note - (rtemp2 + 2) - bass;
            fprintf(fp, "i 2 %f %f 6000 %d 1\n", time, 240 / tempo, note2);
        }

        time = time + nleng;
    }

    fclose(fp);
}

void MainWindow::writeDrums()
{
    //set local variables from UI
    float tempo = ui->leTempo->text().toFloat();
    int length = ui->leNumMeasures->text().toInt();

    const char *f;
    f = (char*)malloc(sizeof(char)*128);
    QByteArray ba = ui->leFileName->text().toLatin1();

    f = ba;

    FILE* fp=fopen(f, "a");

    float loop, time = 0;

    if(ui->cbDrums->currentIndex() == 0)
    {
        for (loop = 0; loop < length; loop += .5)
        {
            fprintf(fp, "i 5 %f %f 20\n", time, 60 / tempo);
            fprintf(fp, "i 4 %f %f 5000\n", time + (60 / tempo), 60 / tempo);
            fprintf(fp, "i 3 %f %f 2000\n", time + (30 / tempo), 60 / tempo / 4);
            fprintf(fp, "i 3 %f %f 2000\n", time + ((30 / tempo) * 2), 60 / tempo / 4);
            fprintf(fp, "i 3 %f %f 2000\n", time + ((30 / tempo) * 3), 60 / tempo / 4);
            fprintf(fp, "i 3 %f %f 2000\n", time + ((30 / tempo) * 4), 60 / tempo / 4);
            time = time + (120 / tempo);
        }
    }

    fprintf(fp, "</CsScore>\n</CsoundSynthesizer>");

    fclose(fp);
}


MainWindow::~MainWindow()
{
    delete ui;
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
