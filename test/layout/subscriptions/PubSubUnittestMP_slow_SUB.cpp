#include "PubSubUnittestMP_slow_SUB.h"
#include "zmq.h"
#include <QtTest>

QTEST_MAIN(TextUnittestMPSub);

TextUnittestMPSub::TextUnittestMPSub(QObject *parent)
    : QObject(parent)
    , mpContext(0)
    , mpSubscriberThread(0)
    , mpSubscriberComp(0)
{}

void TextUnittestMPSub::initTestCase()
{
    mpContext = YaComponent::context_new();
    mpSubscriberThread = new YaComponentThread();
    mpSubscriberComp = new SubscriberComp(mpContext);

    mpSubscriberThread->start();
    mpSubscriberComp->init();
    mpSubscriberComp->setConnectionParaData("tcp://127.0.0.1:42163", "subscriber_slow");
    mpSubscriberComp->moveToThread(mpSubscriberThread);
}

void TextUnittestMPSub::cleanupTestCase()
{
    mpSubscriberComp->close();

    zmq_ctx_term(mpContext);

    if (mpSubscriberThread) {
        mpSubscriberThread->quit();
        QTest::qWait(100);
        QVERIFY(mpSubscriberThread->isFinished());
    }

    mpContext = 0;
    delete mpSubscriberThread;
    mpSubscriberThread = 0;
    delete mpSubscriberComp;
    mpSubscriberComp = 0;
}

void TextUnittestMPSub::testMPMTSub()
{

    testRoutine();

}

void TextUnittestMPSub::testRoutine()
{
    int rc = mpSubscriberComp->setNotifications();
    QVERIFY(-1 != rc);
    QTest::qWait(500);
    rc = mpSubscriberComp->requestStart();
    QVERIFY(-1 != rc);
    auto expectedRuntime = 5000 / YaComponent::TimeOut;
    auto loops = 0;
    do {
        QTest::qWait(YaComponent::TimeOut);
        loops++;

    } while (loops < expectedRuntime);

    mpSubscriberComp->clearNotifications();
    mpSubscriberComp->requestStop();
}

#include "PublisherIfcProxy.h"
#include <thread>
#include <QtCore/QtDebug>

SubscriberComp::SubscriberComp(void *context)
    : IPublisherIfcProxyHandler(self())
    , YaComponent::SubscriberCompImpl(context, static_cast<IPublisherIfcProxyHandler &>(self()))
    , miPropertiesCnt(0)
{
    //  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
}

void SubscriberComp::init()
{
    SubscriberCompImpl::init();
}

int SubscriberComp::setNotifications()
{
    return mData.setNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
}

int SubscriberComp::clearNotifications()
{
    int rc = -1;
    rc = mData.clearNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
    return rc;
}

int SubscriberComp::requestStart()
{
    return mData.requestStartData();
}

int SubscriberComp::requestStop()
{
    return mData.requestStopData();
}

void SubscriberComp::onProperty(int proxyId, const Data &data)
{
    assert(PROXY_DATA == proxyId);
    miPropertiesCnt++;
    if (miPropertiesCnt % 100 == 0) {
        qDebug() << "slow received " << miPropertiesCnt << "last counter" << data.counter();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
