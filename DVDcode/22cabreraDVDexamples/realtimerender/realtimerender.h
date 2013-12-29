
#ifndef REALTIMERENDER_H
#define REALTIMERENDER_H

#include <QMainWindow>
#include <QCloseEvent>

class QComboBox;
class QAction;
class QToolButton;

class RealtimeRender:public QMainWindow
{
  Q_OBJECT

  public:
    RealtimeRender();
    ~RealtimeRender();

  protected slots:
    virtual void start(bool on);

  protected:
    void createActions();
    void fillDeviceBoxes();

    QWidget *mainWidget;
    QComboBox *inputComboBox;
    QComboBox *outputComboBox;
    QToolButton *startButton;
    QAction *startAct;
};

#endif
