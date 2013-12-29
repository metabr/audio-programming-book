
#ifndef OFFLINERENDER_H
#define OFFLINERENDER_H

#include <QMainWindow>

class QAction;
class QLineEdit;
class QToolButton;

class OfflineRender:public QMainWindow
{
  Q_OBJECT

  public:
    OfflineRender();
    ~OfflineRender();

  protected slots:
    virtual int process();
    void browseInputFile();
    void browseOutputFile();

  protected:
    void createActions();

    QWidget *mainWidget;
    QLineEdit *inputFileEdit;
    QLineEdit *outputFileEdit;
    QToolButton *browseInputFileButton;
    QToolButton *browseOutputFileButton;
    QToolButton *processButton;
    QAction *processAct;
    QAction *browseInputFileAct;
    QAction *browseOutputFileAct;
};

#endif
