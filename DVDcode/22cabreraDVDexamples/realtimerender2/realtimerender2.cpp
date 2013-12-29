
#include <QtGui>
#include <portaudio.h>

#include "realtimerender2.h"

#include "../compressor/knee_compressor.c"

RealtimeRender2::RealtimeRender2()
{
  RealtimeRender::RealtimeRender();

  data = (mydata *) calloc(sizeof(mydata),1);
  data_init(data);

  inputSlider = new QSlider(mainWidget);
  inputSlider->resize(320, 20);
  inputSlider->move(20, 140);
  inputSlider->setOrientation(Qt::Horizontal);
  connect(inputSlider, SIGNAL(valueChanged(int)),
          this, SLOT(inputChanged(int)));
  inputLabel = new QLabel(mainWidget);
  inputLabel->resize(100, 30);
  inputLabel->move(40, 160);
  inputLabel->setText(tr("Input Level"));
  thresholdSlider = new QSlider(mainWidget);
  thresholdSlider->resize(20, 100);
  thresholdSlider->move(440, 10);
  connect(thresholdSlider, SIGNAL(valueChanged(int)),
          this, SLOT(thChanged(int)));
  thLabel = new QLabel(mainWidget);
  thLabel->resize(50, 30);
  thLabel->move(435, 115);
  thLabel->setText(tr("Thold"));
  thValueLabel = new QLabel(mainWidget);
  thValueLabel->resize(70, 30);
  thValueLabel->move(420, 145);
  ratioSlider = new QSlider(mainWidget);
  ratioSlider->resize(20, 100);
  ratioSlider->move(500, 10);
  connect(ratioSlider, SIGNAL(valueChanged(int)),
          this, SLOT(ratChanged(int)));
  ratLabel = new QLabel(mainWidget);
  ratLabel->resize(50, 30);
  ratLabel->move(495, 115);
  ratLabel->setText(tr("Ratio"));
  ratValueLabel = new QLabel(mainWidget);
  ratValueLabel->resize(50, 30);
  ratValueLabel->move(495, 145);
  attackKnob = new QDial(mainWidget);
  attackKnob->resize(50, 50);
  attackKnob->move(530, 10);
  connect(attackKnob, SIGNAL(valueChanged(int)),
          this, SLOT(attChanged(int)));
  attLabel = new QLabel(mainWidget);
  attLabel->resize(50, 30);
  attLabel->move(540, 60);
  attLabel->setText(tr("Att"));
  attValueLabel = new QLabel(mainWidget);
  attValueLabel->resize(50, 30);
  attValueLabel->move(540, 80);
  releaseKnob = new QDial(mainWidget);
  releaseKnob->resize(50, 50);
  releaseKnob->move(590, 10);
  connect(releaseKnob, SIGNAL(valueChanged(int)),
          this, SLOT(relChanged(int)));
  relLabel = new QLabel(mainWidget);
  relLabel->resize(50, 30);
  relLabel->move(600, 60);
  relLabel->setText(tr("Rel"));
  relValueLabel = new QLabel(mainWidget);
  relValueLabel->resize(50, 30);
  relValueLabel->move(600, 80);
  kneeKnob = new QDial(mainWidget);
  kneeKnob->resize(50, 50);
  kneeKnob->move(560, 105);
  connect(kneeKnob, SIGNAL(valueChanged(int)),
          this, SLOT(kneeChanged(int)));
  kneeLabel = new QLabel(mainWidget);
  kneeLabel->resize(50, 30);
  kneeLabel->move(570, 150);
  kneeLabel->setText(tr("Knee"));
  kneeValueLabel = new QLabel(mainWidget);
  kneeValueLabel->resize(50, 30);
  kneeValueLabel->move(570, 170);

  resize(660, 200);

  thresholdSlider->setValue(48);
  ratioSlider->setValue(40);
  attackKnob->setValue(24);
  releaseKnob->setValue(29);
  kneeKnob->setValue(48);
}

RealtimeRender2::~RealtimeRender2()
{
  free(data);
}

void RealtimeRender2::start(bool on)
{
  PaError err;
  bool ok;
  PaDeviceIndex devin,devout;
  PaStreamParameters inparam, outparam;
  PaStream *handle;
  float buf[BUF], out[BUF];
  int i;

  if (on) {
    outputComboBox->setEnabled(false);
    inputComboBox->setEnabled(false);

    devin = inputComboBox->itemData(inputComboBox->currentIndex()).toInt();
    devout = outputComboBox->itemData(outputComboBox->currentIndex()).toInt();

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
              for (i = 0; i < BUF; i++) {
                buf[i] = buf[i] * inputlvl;
              }
              audio_process(buf, out, BUF, data);
              err = (int) Pa_WriteStream(handle, out, BUF);
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
    startAct->setChecked(false);
    if (!ok) {
      start(false);
    }
  }
  else {
    outputComboBox->setEnabled(true);
    inputComboBox->setEnabled(true);
  }
}

void RealtimeRender2::inputChanged(int value)
{
  float input = (value + 1.0) / 100.0;
  //mutex.lock();
  inputlvl = input;
  //mutex.unlock();
}

void RealtimeRender2::thChanged(int value)
{
  float th = -(99.0 - value) / 3.0;
  thValueLabel->setText(QString("%1 dB").arg(th, 1, 'f', 1));
  //mutex.lock();
  data->threshold = th;
  //mutex.unlock();
}

void RealtimeRender2::ratChanged(int value)
{
  float rat = 1.0 + (value/ 5.0);
  ratValueLabel->setText(QString("1:%1").arg(rat, 1, 'f', 1));
  //mutex.lock();
  data->ratio = 1.0 - (1.0/rat);
  //mutex.unlock();
}

void RealtimeRender2::attChanged(int value)
{
  int att = (value + 1) * 2;  // + 1 to avoid nan
  attValueLabel->setText(QString("%1 ms").arg(att));
  //mutex.lock();
  data->attack_constant = 1.0 / (att * (float) SR /1000.0);
  //mutex.unlock();
}

void RealtimeRender2::relChanged(int value)
{
  int rel = (value + 1) * 5;  // + 1 to avoid nan
  relValueLabel->setText(QString("%1 ms").arg(rel));
  //mutex.lock();
  data->release_constant = 1.0 / (rel * (float) SR /1000.0);
  //mutex.unlock();
}

void RealtimeRender2::kneeChanged(int value)
{
  float knee = value / 6.0;
  kneeValueLabel->setText(QString("%1 dB").arg(knee, 1, 'f', 1));
  //mutex.lock();
  data->knee = knee;
  //mutex.unlock();
}
