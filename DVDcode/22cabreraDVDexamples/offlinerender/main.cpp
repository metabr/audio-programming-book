
#include <QApplication>
#include "offlinerender.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  OfflineRender * mw = new OfflineRender();
  mw->show();
  return app.exec();
}
