#include "RapidClick.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  RapidClick w;
  w.show();
  return a.exec();
}
