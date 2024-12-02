#include "BasicReqRespTest.h"
#include <QtTest>

BasicReqRespTest::BasicReqRespTest( QObject* parent )
  : QObject( parent )
{
}
void BasicReqRespTest::initComponents()
{
  mpContext = YaComponent::context_new();
  mpWorker = new WorkerComp( mpContext );
  mpControl = new ControlComp( mpContext );

  mpWorkerThread = new QThread();
  mpWorkerThread->start();

  mpControlThread = new QThread();
  mpControlThread->start();

  mpWorker->init();
  mpWorker->setConnectionParaWorker( "inproc:///tmp/workercml", 5000 );
  mpWorker->moveToThread( mpWorkerThread );

  mpControl->init();
  mpControl->setConnectionParaWorker( "inproc:///tmp/workercml", "Control" );
  mpControl->moveToThread( mpControlThread );
}

void BasicReqRespTest::cleanupTestCase() {}

void BasicReqRespTest::cleanupComponents()
{
  mpControl->close();
  mpWorker->close();

  if ( mpWorkerThread )
  {
    mpWorkerThread->quit();
    QTest::qWait( 100 );
    QVERIFY( mpWorkerThread->isFinished() );
  }

  if ( mpControlThread )
  {
    mpControlThread->quit();
    QTest::qWait( 100 );
    QVERIFY( mpControlThread->isFinished() );
  }
  delete mpWorker;
  mpWorker = 0;
  delete mpWorkerThread;
  mpWorkerThread = 0;
  delete mpControl;
  mpControl = 0;
  delete mpControlThread;
  mpControlThread = 0;
  YaComponent::context_term( mpContext );
  mpContext = 0;
}

void BasicReqRespTest::testSingleReqResp()
{
  initComponents();

  mpControl->sendReq( std::string( "what" ) );

  QVERIFY( QTest::qWaitFor( [this]() { return 1 == mpControl->messageCount(); }, 1000 ) );
  QCOMPARE( std::string( "what 1" ), mpControl->message().msg() );

  cleanupComponents();
}

void BasicReqRespTest::testReqRespRepeated()
{
  initComponents();

  for ( auto idx = 0; idx < 50; idx++ )
  {
    mpControl->sendReq( std::string( "what" ) );
    QVERIFY( QTest::qWaitFor( [=]() { return ( idx + 1 ) == mpControl->messageCount(); }, 1000 ) );
    QCOMPARE( QString::fromStdString( std::string( "what " ) + std::to_string( idx + 1 ) ), QString::fromStdString( mpControl->message().msg() ) );
  }
  cleanupComponents();
}

void BasicReqRespTest::testMaxMessageSize()
{
  initComponents();
  std::string s( YaComponent::MaxMessageSize + 10, 'a' ); // s == "aaaaa"
  auto rc = mpControl->sendReq( s );

  QVERIFY( -1 != rc );
  QVERIFY( QTest::qWaitFor( [this]() { return 1 == mpControl->messageCount(); }, 1000 ) );
  QCOMPARE( s + std::string( " 1" ), mpControl->message().msg() );

  cleanupComponents();
}
