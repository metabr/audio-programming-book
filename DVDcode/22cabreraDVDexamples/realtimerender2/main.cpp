
#include <QApplication>
#include "realtimerender2.h"

int main(int argc, char *argv[])
{
      QApplication app(argc, argv);
      RealtimeRender2 * mw = new RealtimeRender2();
      mw->show();
      return app.exec();
}

