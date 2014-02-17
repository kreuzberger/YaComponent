
#include "TextExampleMPMT.h"
//#include "YaComponent.h"
#include <stdio.h>
#include <QtGui/QApplication>

#include "textgen/textgencomp.h"

int main(int argc, char** argv)
{
  QApplication app(argc,argv);
  #include "TextExampleMPMTTextGenProcess.cpp"
  return app.exec();
}
