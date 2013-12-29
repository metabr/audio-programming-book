
#include <QtGui>
#include "offlinerender.h"

#include "../lazzarini/snd_defs.h"

OfflineRender::OfflineRender()
{
  createActions();

  mainWidget = new QWidget;
  inputFileEdit = new QLineEdit(mainWidget);
  inputFileEdit->resize(380, 30);
  inputFileEdit->move(10, 10);
  outputFileEdit = new QLineEdit(mainWidget);
  outputFileEdit->resize(380, 30);
  outputFileEdit->move(10, 40);
  browseInputFileButton = new QToolButton(mainWidget);
  browseInputFileButton->resize(60, 30);
  browseInputFileButton->move(395, 10);
  browseInputFileButton->setDefaultAction(browseInputFileAct);
  browseOutputFileButton = new QToolButton(mainWidget);
  browseOutputFileButton->resize(60, 30);
  browseOutputFileButton->move(395, 40);
  browseOutputFileButton->setDefaultAction(browseOutputFileAct);
  processButton = new QToolButton(mainWidget);
  processButton->resize(100, 30);
  processButton->move(150, 80);
  processButton->setDefaultAction(processAct);

  setCentralWidget(mainWidget);
  resize(460, 130);
}

OfflineRender::~OfflineRender()
{
}

void OfflineRender::createActions()
{
  processAct = new QAction(tr("Process"), this);
  processAct->setShortcut(tr("Ctrl+P"));
  processAct->setStatusTip(tr("Process file"));
  connect(processAct, SIGNAL(triggered()), this, SLOT(process()));

  browseInputFileAct = new QAction(tr("Browse"), this);
  browseInputFileAct->setShortcut(tr("Ctrl+I"));
  browseInputFileAct->setStatusTip(tr("Browse Input File"));
  connect(browseInputFileAct, SIGNAL(triggered()),
          this, SLOT(browseInputFile()));

  browseOutputFileAct = new QAction(tr("Browse"), this);
  browseOutputFileAct->setShortcut(tr("Ctrl+U"));
  browseOutputFileAct->setStatusTip(tr("Browse Output File"));
  connect(browseOutputFileAct, SIGNAL(triggered()),
          this, SLOT(browseOutputFile()));
}

int OfflineRender::process()
{
  // taken mostly from the main4.cpp file
  SNDFILE *psfo, *psfi;
  float *buffer;
  int chans, cnt=0, pt=0, frames=0;
  float sr, dtime, fb, *del;
  dtime = 0.01;
  fb = 0.8;

  // allocate buffer, delay & table memory
  buffer = new float[def_vsize];
  del = new float[(int)(dtime*def_sr)];
  memset(del, 0, sizeof(float)*(int)(dtime*def_sr));

  // now we open the files
  if(!(psfi = soundin_open(inputFileEdit->text().toLocal8Bit().data(),
                           &chans, &sr, &frames))){
    qDebug() << "error opening input file" << inputFileEdit->text();
    return -1;
  }

  if(chans > 1 || sr != def_sr) {
    qDebug("unsupported channels or sr");
    return -1;
  }

  if(!(psfo = 
      soundout_open(outputFileEdit->text().toLocal8Bit().data()) )) {
    qDebug() <<"error opening output file" << outputFileEdit->text();
    return -1;
  }
  // clip instead of wrap
  sf_command(psfo, SFC_SET_CLIPPING, NULL, SF_TRUE);
  do {
    cnt = soundin(psfi, buffer);
    flanger(buffer, dtime, fb, dtime, del, &pt);
    soundout(psfo,buffer, cnt);
  } while(cnt);

  // close file & free memory
  soundin_close(psfi);
  soundout_close(psfo);
  delete[] buffer;
  delete[] del;

  return 0;
}

void OfflineRender::browseInputFile()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                 tr("Input File"), "",
                                 tr("Audio Files (*.wav *.aif)"));
  if (fileName != "")
    inputFileEdit->setText(fileName);
}

void OfflineRender::browseOutputFile()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("Output File"), "",
                                 tr("Audio Files (*.wav *.aif)"));
  if (fileName != "")
    outputFileEdit->setText(fileName);
}

