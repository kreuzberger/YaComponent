#include "PubSubUnittestSP.h"
#include "zmq.h"

QTEST_MAIN(PubSubUnittestSP);

PubSubUnittestSP::PubSubUnittestSP(QObject *parent)
    : QObject(parent)
{}

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
    mPublisherThread = std::make_unique<YaComponentThread>();
    mSubscriperThread = std::make_unique<YaComponentThread>();
    mSubScriber.reset(new SubscriberComp(mContext));
    mPublisher.reset(new PublisherComp(mContext));

    mPublisherThread->start();
    mSubscriperThread->start();
    QTest::qWait(10);
    mSubScriber->init();
    mPublisher->init();
    mPublisher->setConnectionParaReceiverData("inproc://testinprocsubpub", 3000);
    mPublisher->miMaxMessageCnt = 12 * 1024;

    mSubScriber->setConnectionParaData("inproc://testinprocsubpub", "subscriber_sp");
    mPublisher->moveToThread(mPublisherThread.get());
    mSubScriber->moveToThread(mSubscriperThread.get());
}

void PubSubUnittestSP::cleanupComponents()
{
    mPublisher->close();
    mSubScriber->close();
    mPublisherThread->quit();
    mSubscriperThread->quit();
    QTest::qWait(100);
    QVERIFY(mPublisherThread->isFinished());
    QVERIFY(mSubscriperThread->isFinished());
    YaComponent::context_term(mContext);
}

void PubSubUnittestSP::testConnect()
{
    int rc = -1;
    rc = mSubScriber->setNotifications();
    QVERIFY(-1 != rc);
    QTest::qWait(50);

    Data data;
    data.set_timeseconds(0);
    data.set_timefraction(0.0);
    data.set_counter(123);
    rc = mPublisher->sendData(data);
    QVERIFY(-1 != rc);
    QTest::qWait(50);
    QCOMPARE(mSubScriber->miPropertiesCnt, 1);
    QCOMPARE(mSubScriber->mLastData.counter(), 123);
}

void PubSubUnittestSP::testSendWithoutSubscriptions()
{
    Data data;
    data.set_timeseconds(0);
    data.set_timefraction(0.0);
    data.set_counter(123);
    auto rc = mPublisher->sendData(data);
    QVERIFY(-1 == rc);
    QCOMPARE(mSubScriber->miPropertiesCnt, 0);
}

void PubSubUnittestSP::testSendLVCConnect()
{
    auto cnt = 10;
    for (auto i = 0; i < cnt; i++) {
        Data data;
        data.set_timeseconds(0);
        data.set_timefraction(0.0);
        data.set_counter(i);
        mPublisher->sendData(data);
        QTest::qWait(10);
    }

    int rc = -1;
    rc = mSubScriber->setNotifications();
    QVERIFY(-1 != rc);
    QTest::qWait(50);
    QCOMPARE(mSubScriber->miPropertiesCnt, 1);
    QCOMPARE(mSubScriber->mLastData.counter(), 9);
}

void PubSubUnittestSP::testSendLVCConnectEmpty()
{
    int rc = -1;
    rc = mSubScriber->setNotifications();
    QVERIFY(-1 != rc);
    QTest::qWait(50);
    QCOMPARE(mSubScriber->miPropertiesCnt, 0);
    QCOMPARE(mSubScriber->mLastData.counter(), 0);
}
