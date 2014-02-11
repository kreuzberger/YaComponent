#include "BasicLayout.h"
#include "YaComponent.h"

#include <QtCore/QCoreApplication>
#include <stdio.h>

#include "textoutcomp.h"

int main(int argc, char** argv)
{
  QCoreApplication app(argc,argv);
  #include "BasicLayoutprocess2.cpp"
  app.exec();
}
