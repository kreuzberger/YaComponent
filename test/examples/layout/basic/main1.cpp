
#include "BasicLayout.h"
#include "YaComponent.h"
#include <stdio.h>
#include <QtGui/QApplication>

int main(int argc, char** argv)
{
  QApplication app(argc,argv);
  #include "BasicLayoutprocess1.cpp"
  return app.exec();
}