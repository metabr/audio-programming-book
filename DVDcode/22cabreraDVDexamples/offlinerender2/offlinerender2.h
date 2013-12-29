
#ifndef OFFLINERENDER2_H
#define OFFLINERENDER2_H

#include <QtGui>

#include "../offlinerender/offlinerender.h"

class OfflineRender2:public OfflineRender
{
  Q_OBJECT

  public:
    OfflineRender2();
    ~OfflineRender2();

  protected slots:
    virtual int process();

  protected:
    QSlider *timeSlider;
    QLabel *timeLabel;
    QSlider *fbSlider;
    QLabel *fbLabel;
    QProgressBar *progressBar;
};

#endif

