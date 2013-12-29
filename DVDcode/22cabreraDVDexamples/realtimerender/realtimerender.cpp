
#include <QtGui>
#include <portaudio.h>

#include "realtimerender.h"

#define SR 44100
#define BUF 512
#define END 60

RealtimeRender::RealtimeRender()
{
  createActions();

  mainWidget = new QWidget;
  inputComboBox = new QComboBox(mainWidget);
  inputComboBox->resize(380, 30);
  inputComboBox->move(10, 10);
  outputComboBox = new QComboBox(mainWidget);
  outputComboBox->resize(380, 30);
  outputComboBox->move(10, 40);
  startButton = new QToolButton(mainWidget);
  startButton->resize(80, 30);
  startButton->move(150, 80);
  startButton->setDefaultAction(startAct);
  QLabel *warningLabel = new QLabel(mainWidget);
  warningLabel->move(10, 120);
  warningLabel->setText(tr("Warning! Feedback is likely!"));

  setCentralWidget(mainWidget);
  resize(400, 140);
  fillDeviceBoxes();
}

RealtimeRender::~RealtimeRender()
{
}

void RealtimeRender::fillDeviceBoxes()
{
  // mostly from echo.c
  PaError err;
  PaDeviceIndex ndev;
  const PaDeviceInfo *info;
  int i;

  inputComboBox->clear();
  outputComboBox->clear();

  err = Pa_Initialize();
  if (err == paNoError) {
    ndev = Pa_GetDeviceCount();
    for (i=0; i<ndev; i++) {
      info = Pa_GetDeviceInfo((PaDeviceIndex) i);
      if (info->maxOutputChannels > 0) {
        outputComboBox->addItem(info->name, i);
      }
      if (info->maxInputChannels > 0) {
        inputComboBox->addItem(info->name, i);
      }
    }
  }
  else {
    qDebug() << "Error initializing PortAudio!";
  }
  Pa_Terminate();
}

void RealtimeRender::start(bool on)
{
  PaError err;
  bool ok;
  PaDeviceIndex devin,devout;
  PaStreamParameters inparam, outparam;
  PaStream *handle;
  float buf[BUF];

  if (on) {
    outputComboBox->setEnabled(false);
    inputComboBox->setEnabled(false);

    devin = inputComboBox->
        itemData(inputComboBox->currentIndex()).toInt();
    devout = outputComboBox->
        itemData(outputComboBox->currentIndex()).toInt();

    err = Pa_Initialize();
    ok = true;
    if(err == paNoError){
      memset(&inparam, 0, sizeof(PaStreamParameters));
      inparam.device = (PaDeviceIndex) devin;
      inparam.channelCount = 1;
      inparam.sampleFormat = paFloat32;
      memset(&outparam, 0, sizeof(PaStreamParameters));
      outparam.device = (PaDeviceIndex) devout;
      outparam.channelCount = 1;
      outparam.sampleFormat = paFloat32;

      err = Pa_OpenStream(&handle,&inparam,&outparam,SR,BUF,paNoFlag,
                          NULL, NULL);
      if(err == paNoError){
        err = Pa_StartStream(handle);
        if(err == paNoError){
          while(Pa_GetStreamTime(handle) < END){
            err = Pa_ReadStream(handle, buf, BUF);
            if(err == paNoError){
    // No process for now, just pass through
              err = (int) Pa_WriteStream(handle, buf, BUF);
              if(err != paNoError) {
                ok = false;
                qDebug("%s", Pa_GetErrorText(err));
              }
            }
            else {
              ok = false;
              qDebug("%s", Pa_GetErrorText(err));
            }
            qApp->processEvents();
            if (!startAct->isChecked()) {
              start(false);
              break;
            };
          }
          Pa_StopStream(handle);
        }
        else {
          ok = false;
          qDebug("%s", Pa_GetErrorText(err));
        }
        Pa_CloseStream(handle);
      }
      else {
        ok = false;
        qDebug("%s", Pa_GetErrorText(err));
      }
      Pa_Terminate();
    }
    else {
      ok = false;
      qDebug("%s", Pa_GetErrorText(err));
    }
    if (!ok) {
      startAct->setChecked(false);
      start(false);
    }
  }
  else {
    outputComboBox->setEnabled(true);
    inputComboBox->setEnabled(true);
  }
}

void RealtimeRender::createActions()
{
  startAct = new QAction(tr("Start"), this);
  startAct->setShortcut(tr("Ctrl+R"));
  startAct->setStatusTip(tr("Process audio in realtime"));
  startAct->setCheckable(true);
  connect(startAct, SIGNAL(toggled(bool)), this, SLOT(start(bool)));
}

