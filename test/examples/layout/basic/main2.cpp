#include "BasicLayout.h"
#include "YaComponent.h"

#include <QtCore/QCoreApplication>
#include <stdio.h>

int main(int argc, char** argv)
{
  QCoreApplication app(argc,argv);
  #include "BasicLayoutprocess2.cpp"
  app.exec();
}
