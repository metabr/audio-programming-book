
#ifndef REALTIMERENDER2_H
#define REALTIMERENDER2_H

#include "../realtimerender/realtimerender.h"

class QSlider;
class QDial;
class QLabel;

#include <QMutex>

#define LOOKAHEAD_SAMPLES 1024
#define RMS_WINDOW_SAMPLES 2048

typedef struct _mydata
{
  float threshold; // in dB FS value
  float ratio; // inverse ratio factor
  float attack_constant;
  float release_constant;
  float knee; //knee width in dB FS
  float env; // to pass to the next block in linear amplitude
  float delay[LOOKAHEAD_SAMPLES];
  unsigned int delay_position;
  float rms_delay[RMS_WINDOW_SAMPLES];
  unsigned int rms_delay_position;
  float rms_accumulator;
} mydata;

class RealtimeRender2:public RealtimeRender
{
  Q_OBJECT

  public:
    RealtimeRender2();
    ~RealtimeRender2();

  protected slots:
    virtual void start(bool on);
    void inputChanged(int value);
    void thChanged(int value);
    void ratChanged(int value);
    void attChanged(int value);
    void relChanged(int value);
    void kneeChanged(int value);

  protected:
    mydata *data;
    float inputlvl;

    QSlider *inputSlider;
    QLabel *inputLabel;
    QSlider *thresholdSlider;
    QLabel *thLabel;
    QLabel *thValueLabel;
    QSlider *ratioSlider;
    QLabel *ratLabel;
    QLabel *ratValueLabel;
    QDial *attackKnob;
    QLabel *attLabel;
    QLabel *attValueLabel;
    QDial *releaseKnob;
    QLabel *relLabel;
    QLabel *relValueLabel;
    QDial *kneeKnob;
    QLabel *kneeLabel;
    QLabel *kneeValueLabel;

    //QMutex mutex;
};

#endif
