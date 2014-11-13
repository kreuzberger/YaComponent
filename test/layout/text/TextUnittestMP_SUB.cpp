#include "TextUnittestMP_SUB.h"
#include "zmq.h"
#include <QtTest>

QTEST_MAIN(TextUnittestMPSub);

TextUnittestMPSub::TextUnittestMPSub(QObject *parent)
  : QObject(parent)
  , mpContext(0)
  , mpTextOutThread(0)
  , mpTextOut( 0 )
{
}

void TextUnittestMPSub::initComponentsMPMT()
{

  mpContext = YaComponent::context_new();
  mpTextOutThread = new YaComponentThread();
  mpTextOut = new TextOutComp( mpContext );

  mpTextOutThread->start();
  mpTextOut->init();
  mpTextOut->setConnectionParaXml( "tcp://127.0.0.1:42163", "tcp://127.0.0.1:42164","TextOut" );
  mpTextOut->moveToThread(mpTextOutThread);
}

void TextUnittestMPSub::cleanupTestCase()
{

}

void TextUnittestMPSub::cleanupComponents()
{
  mpTextOut->close();

  zmq_ctx_term( mpContext );


  if( mpTextOutThread )
  {
    mpTextOutThread->quit();
    QTest::qWait(100);
    QVERIFY(mpTextOutThread->isFinished());
  }

  mpContext = 0;
  delete mpTextOutThread;
  mpTextOutThread = 0;
  delete mpTextOut;
  mpTextOut = 0;
}


void TextUnittestMPSub::testMPMTSub()
{
  initComponentsMPMT();

  testRoutine();

  cleanupComponents();
}


void TextUnittestMPSub::testRoutine()
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
