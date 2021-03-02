#include "RapidClick.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
  QCoreApplication::setOrganizationName("BlackledgeBuilds");
  QCoreApplication::setApplicationName("RapidClick");
  QApplication a(argc, argv);
  RapidClick w;
  w.show();
  return a.exec();
}
