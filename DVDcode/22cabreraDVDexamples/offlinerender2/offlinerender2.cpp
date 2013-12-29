
#include <QtGui>
#include "offlinerender2.h"

#include "../lazzarini/snd_defs.h"

OfflineRender2::OfflineRender2()
{
  // Call constructor of parent
  OfflineRender::OfflineRender();

  timeSlider = new QSlider(mainWidget);
  timeSlider->resize(20, 100);
  timeSlider->move(460, 10);
  timeSlider->setValue(25);
  timeLabel = new QLabel(mainWidget);
  timeLabel->resize(50, 30);
  timeLabel->move(455, 115);
  timeLabel->setText(tr("Time"));
  fbSlider = new QSlider(mainWidget);
  fbSlider->resize(20, 100);
  fbSlider->move(500, 10);
  fbSlider->setValue(80);
  fbLabel = new QLabel(mainWidget);
  fbLabel->resize(50, 30);
  fbLabel->move(495, 115);
  fbLabel->setText(tr("Fback"));
  progressBar = new QProgressBar(mainWidget);
  progressBar->resize(520, 30);
  progressBar->move(10, 140);

  resize(540, 180);
}

OfflineRender2::~OfflineRender2()
{
}

int OfflineRender2::process()
{
  // taken mostly from the main4.cpp file
  SNDFILE *psfo, *psfi;
  float *buffer;
  int chans, cnt=0, pt=0, frames=0, c=0;
  float sr, dtime, fb, *del;
  dtime = (timeSlider->value() + 1.0) / 2000.0;
  fb = fbSlider->value() / 100.0;

  // allocate buffer, delay & table memory
  buffer = new float[def_vsize];
  del = new float[(int)(dtime*def_sr)];
  memset(del, 0, sizeof(float)*(int)(dtime*def_sr));

  // open the files
  if(!(psfi = soundin_open(inputFileEdit->text().toLocal8Bit().data(),
                           &chans, &sr, &frames))){
    qDebug() << "error opening input file" << inputFileEdit->text();
    return -1;
  }

  if(chans > 1 || sr != def_sr) {
    qDebug("unsupported channels or sr");
    return -1;
  }

  if(!(psfo = soundout_open(outputFileEdit->text().toLocal8Bit().data()))){
    qDebug() << "error opening output file" << outputFileEdit->text();
    return -1;
  }
  // clip instead of wrap
  sf_command(psfo, SFC_SET_CLIPPING, NULL, SF_TRUE);
  progressBar->setRange(0,frames);
  progressBar->setValue(0);
  do {
    cnt = soundin(psfi, buffer);
    flanger(buffer, dtime, 0.8f, dtime,  del, &pt);
    soundout(psfo,buffer, cnt);
    progressBar->setValue(progressBar->value() + cnt);
    if (c++%200 == 0)
      qApp->processEvents();
  } while(cnt);

  // close file & free memory
  soundin_close(psfi);
  soundout_close(psfo);
  delete[] buffer;
  delete[] del;

  return 0;
}

