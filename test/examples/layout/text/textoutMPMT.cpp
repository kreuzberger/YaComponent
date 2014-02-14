#include "TextExampleMPMT.h"
//#include "YaComponent.h"

#include <QtCore/QCoreApplication>
#include <stdio.h>

#include "textout/textoutcomp.h"

int main(int argc, char** argv)
{
  QCoreApplication app(argc,argv);
  #include "TextExampleMPMTTextOutProcess.cpp"
  app.exec();
}
