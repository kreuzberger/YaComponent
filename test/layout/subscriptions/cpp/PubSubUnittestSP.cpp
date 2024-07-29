#include "PubSubUnittestSP.h"
#include "zmq.h"

QTEST_MAIN( PubSubUnittestSP );

PubSubUnittestSP::PubSubUnittestSP( QObject* parent )
  : QObject( parent )
{
}

void PubSubUnittestSP::initTestCase() {}
void PubSubUnittestSP::cleanupTestCase() {}

void PubSubUnittestSP::init()
{
  initComponents();
}

void PubSubUnittestSP::cleanup()
{
  cleanupComponents();
}

void PubSubUnittestSP::initComponents()
{
  mContext = YaComponent::context_new();
  mPublisherThread = std::make_unique<QThread>();
  mSubscriperThread = std::make_unique<QThread>();
  mSubScriber.reset( new SubscriberComp( mContext ) );
  mPublisher.reset( new PublisherComp( mContext ) );

  mPublisherThread->start();
  mSubscriperThread->start();
  QTest::qWait( 10 );
  mSubScriber->init();
  mPublisher->init();
  mPublisher->setConnectionParaReceiverData( "inproc://testinprocsubpub", 3000 );

  mSubScriber->setConnectionParaData( "inproc://testinprocsubpub", "subscriber_sp" );
  mPublisher->moveToThread( mPublisherThread.get() );
  mSubScriber->moveToThread( mSubscriperThread.get() );
}

void PubSubUnittestSP::cleanupComponents()
{
  mPublisher->close();
  mSubScriber->close();
  mPublisherThread->quit();
  mSubscriperThread->quit();
  QTest::qWait( 100 );
  QVERIFY( mPublisherThread->isFinished() );
  QVERIFY( mSubscriperThread->isFinished() );
  YaComponent::context_term( mContext );
}

void PubSubUnittestSP::testNotification()
{
  int rc = -1;
  rc = mSubScriber->setNotifications();
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );

  Data data;
  data.set_timeseconds( 0 );
  data.set_timefraction( 0.0 );
  data.set_counter( 123 );
  rc = mPublisher->sendData( data );
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesCnt, 1 );
  QCOMPARE( mSubScriber->mLastData.counter(), 123 );
}

void PubSubUnittestSP::testSendWithoutSubscriptions()
{
  Data data;
  data.set_timeseconds( 0 );
  data.set_timefraction( 0.0 );
  data.set_counter( 123 );
  auto rc = mPublisher->sendData( data );
  QTest::qWait( 50 );
  QVERIFY( -1 == rc );
  QCOMPARE( mSubScriber->miPropertiesCnt, 0 );
}

void PubSubUnittestSP::testSendLVCConnect()
{
  auto cnt = 10;
  for ( auto i = 0; i < cnt; i++ )
  {
    Data data;
    data.set_timeseconds( 0 );
    data.set_timefraction( 0.0 );
    data.set_counter( i );
    mPublisher->sendData( data );
    QTest::qWait( 10 );
  }

  int rc = -1;
  rc = mSubScriber->setNotifications();
  QVERIFY( -1 != rc );
  QVERIFY( QTest::qWaitFor( [this]() { return 1 == mSubScriber->miPropertiesCnt; } ) );
  QCOMPARE( mSubScriber->miPropertiesCnt, 1 );
  QCOMPARE( mSubScriber->mLastData.counter(), 9 );
}

void PubSubUnittestSP::testSendLVCConnectEmpty()
{
  int rc = -1;
  rc = mSubScriber->setNotifications();
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesCnt, 0 );
  QCOMPARE( mSubScriber->mLastData.counter(), 0 );
}

void PubSubUnittestSP::testSendOnChangeOff()
{
  // send identical value twice, should be received a second time
  int rc = -1;
  rc = mSubScriber->setNotifications();
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );

  Data data;
  data.set_timeseconds( 0 );
  data.set_timefraction( 0.0 );
  data.set_counter( 123 );
  rc = mPublisher->sendData( data );
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesCnt, 1 );
  QCOMPARE( mSubScriber->mLastData.counter(), 123 );
  mSubScriber->mLastData.Clear();

  rc = mPublisher->sendData( data );
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesCnt, 2 );
  QCOMPARE( mSubScriber->mLastData.counter(), 123 );
}

void PubSubUnittestSP::testSendOnChangeOn()
{
  int rc = -1;
  rc = mSubScriber->setNotifications();
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );

  Time time;
  time.set_timeseconds( 0 );
  time.set_timefraction( 0.0 );
  time.set_counter( 1 );
  rc = mPublisher->sendTime( time );
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesTimeCnt, 1 );
  QCOMPARE( mSubScriber->mLastTime.counter(), 1 );
  mSubScriber->mLastTime.Clear();
  // resend time to check
  rc = mPublisher->sendTime( time );
  QVERIFY( -1 != rc );
  QTest::qWait( 50 );
  QCOMPARE( mSubScriber->miPropertiesTimeCnt, 1 );
  QCOMPARE( mSubScriber->mLastTime.counter(), 0 );
}
