
#include <QApplication>
#include "realtimerender.h"

int main(int argc, char *argv[])
{
      QApplication app(argc, argv);
      RealtimeRender * mw = new RealtimeRender();
      mw->show();
      return app.exec();
}

