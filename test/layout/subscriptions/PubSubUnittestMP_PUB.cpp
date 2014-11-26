#include "PubSubUnittestMP_PUB.h"
#include "zmq.h"
#include <QtTest>

QTEST_MAIN(PubSubUnittestMPPub);

PubSubUnittestMPPub::PubSubUnittestMPPub(QObject *parent)
  : QObject(parent)
  , mpContext(0)
  , mpPublisherThread(0)
  , mpPublisher( 0 )
{
}


void PubSubUnittestMPPub::initComponentsMPMT()
{
  mpContext = YaComponent::context_new();
  mpPublisher = new PublisherComp ( mpContext );
  mpPublisherThread = new YaComponentThread();

  mpPublisherThread->start();

  mpPublisher->init();
  mpPublisher->setConnectionParaReceiverData( "tcp://*:42163", 5000 );
  mpPublisher->miMaxMessageCnt = 12522;
  mpPublisher->moveToThread(mpPublisherThread);

}
void PubSubUnittestMPPub::cleanupTestCase()
{

}

void PubSubUnittestMPPub::cleanupComponents()
{
  mpPublisher->close();

  zmq_ctx_term( mpContext );

  if( mpPublisherThread)
  {
    mpPublisherThread->quit();
    QTest::qWait(100);
    QVERIFY(mpPublisherThread->isFinished());
  }

  mpContext = 0;
  delete mpPublisherThread;
  mpPublisherThread = 0;
  delete mpPublisher;
  mpPublisher = 0;
}

void PubSubUnittestMPPub::testMPMTPub()
{
  initComponentsMPMT();

  testRoutine();
  cleanupComponents();
}


void PubSubUnittestMPPub::testRoutine()
{
  QVERIFY( 0 == mpPublisher->miRequestStop );
  do
  {
    QTest::qWait(10);

  } while( ! mpPublisher->mbFinished );

  QTest::qWait(1000);

}
