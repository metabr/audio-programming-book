
#include <QApplication>
#include "offlinerender2.h"

int main(int argc, char *argv[])
{
      QApplication app(argc, argv);
      OfflineRender2 * mw = new OfflineRender2();
      mw->show();
      return app.exec();
}

