#include "BasicReqRespGuiTest.h"
#include <QtTest>

BasicReqRespGuiTest::BasicReqRespGuiTest( QObject* parent )
  : BasicReqRespTest( parent )
{
}
void BasicReqRespGuiTest::initComponents()
{
  mpContext = YaComponent::context_new();
  mpWorker = new WorkerComp( mpContext );
  mpControl = new ControlComp( mpContext );

  mpWorker->init();
  mpWorker->setConnectionParaWorker( "inproc:///tmp/workercml", 5000 );

  mpControl->init();
  mpControl->setConnectionParaWorker( "inproc:///tmp/workercml", "Control" );
}

void BasicReqRespGuiTest::cleanupComponents()
{
  mpControl->close();
  mpWorker->close();

  delete mpWorker;
  mpWorker = 0;
  delete mpControl;
  mpControl = 0;
  YaComponent::context_term( mpContext );
  mpContext = 0;
}
