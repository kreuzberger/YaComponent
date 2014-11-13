#include "TextUnittest.h"
#include "zmq.h"
#include <QtTest>

QTEST_MAIN(TextUnittest);

TextUnittest::TextUnittest(QObject *parent)
  : QObject(parent)
  , mpContext(0)
  , mpTextGenThread(0)
  , mpTextGen1( 0 )
  , mpTextOutThread(0)
  , mpTextOut( 0 )
{
}
void TextUnittest::initComponentsSPST()
{
  mpContext = YaComponent::context_new();
  mpTextGen1 = new TextGenComp ( mpContext );
  mpTextOut = new TextOutComp( mpContext );

  mpTextGen1->init();
  mpTextGen1->setConnectionParaTextGenXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync", 5000 );
  mpTextGen1->setConnectionParaTextGenPlain( "inproc:///tmp/hurtmeplain", "inproc:///tmp/hurtmeplainsync", 5000 );
  mpTextOut->init();
  mpTextOut->setConnectionParaXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync","TextOut" );
}

void TextUnittest::initComponentsSPMT()
{

  mpContext = YaComponent::context_new();
  mpTextGenThread = new YaComponentThread();
  mpTextGen1 = new TextGenComp ( mpContext );
  mpTextOutThread = new YaComponentThread();
  mpTextOut = new TextOutComp( mpContext );

  mpTextGenThread->start();
  mpTextGen1->init();
  mpTextGen1->setConnectionParaTextGenXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync", 5000 );
  mpTextGen1->setConnectionParaTextGenPlain( "inproc:///tmp/hurtmeplain", "inproc:///tmp/hurtmeplainsync", 5000 );
  mpTextGen1->moveToThread(mpTextGenThread);
  mpTextOutThread->start();
  mpTextOut->init();
  mpTextOut->setConnectionParaXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync","TextOut" );
  mpTextOut->moveToThread(mpTextOutThread);
}


void TextUnittest::initComponentsSPMTGui()
{

  mpContext = YaComponent::context_new();
  mpTextGenThread = new YaComponentThread();
  mpTextGen1 = new TextGenComp ( mpContext );
  mpTextOutThread = new YaComponentThread();
  mpTextOut = new TextOutComp( mpContext );

  mpTextGenThread->start();
  mpTextGen1->init();
  mpTextGen1->setConnectionParaTextGenXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync", 5000 );
  mpTextGen1->setConnectionParaTextGenPlain( "inproc:///tmp/hurtmeplain", "inproc:///tmp/hurtmeplainsync", 5000 );
  mpTextGen1->moveToThread(mpTextGenThread);
  mpTextOutThread->start();
  mpTextOut->init();
  mpTextOut->setConnectionParaXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync","TextOut" );
}

void TextUnittest::cleanupTestCase()
{

}

void TextUnittest::cleanupComponents()
{
  mpTextOut->close();
  mpTextGen1->close();


  if( mpTextGenThread)
  {
    mpTextGenThread->quit();
    QTest::qWait(100);
    QVERIFY(mpTextGenThread->isFinished());
  }

  if( mpTextOutThread )
  {
    mpTextOutThread->quit();
    QTest::qWait(100);
    QVERIFY(mpTextOutThread->isFinished());
  }
  delete mpTextGen1;
  mpTextGen1 = 0;
  delete mpTextGenThread;
  mpTextGenThread = 0;
  delete mpTextOut;
  mpTextOut = 0;
  delete mpTextOutThread;
  mpTextOutThread = 0;
  zmq_ctx_term( mpContext );
  mpContext = 0;

}

void TextUnittest::testSPST()
{
  initComponentsSPST();

  testRoutine();
  cleanupComponents();
}

void TextUnittest::testSPMT()
{
  initComponentsSPMT();

  testRoutine();

  cleanupComponents();
}

void TextUnittest::testSPMTGui()
{
  initComponentsSPMTGui();

  testRoutine();

  cleanupComponents();
}


void TextUnittest::testRoutine()
{
  mpTextOut->setNotifications();
  mpTextOut->requestStart();

  for( int i=1;i<3;i++)
  {
    QTest::qWait(YaComponent::TimeOut);
  }

  mpTextOut->requestStop();

  for( int i=1;i<3;i++)
  {
    QTest::qWait(YaComponent::TimeOut);
  }

  QCOMPARE(mpTextOut->miPropertiesCnt,1);
  QCOMPARE(mpTextOut->miResponseStartCnt,1);
  QCOMPARE(mpTextOut->miResponseStopCnt,1);
}
