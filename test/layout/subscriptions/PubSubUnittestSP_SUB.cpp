#include "PubSubUnittestSP_SUB.h"
#include "zmq.h"
#include <QtTest>

// void TextUnittestSPSub::initComponentsMPMT()
// {
//     mpContext = YaComponent::context_new();
//     mpSubscriberThread = new YaComponentThread();
//     mpSubscriberComp = new SubscriberComp(mpContext);

//     mpSubscriberThread->start();
//     mpSubscriberComp->init();
//     mpSubscriberComp->setConnectionParaData("tcp://127.0.0.1:42163", "subscriber_fast");
//     mpSubscriberComp->moveToThread(mpSubscriberThread);
// }

// void TextUnittestSPSub::cleanupComponents()
// {
//     mpSubscriberComp->close();

//     zmq_ctx_term(mpContext);

//     if (mpSubscriberThread) {
//         mpSubscriberThread->quit();
//         QTest::qWait(100);
//         QVERIFY(mpSubscriberThread->isFinished());
//     }

//     mpContext = 0;
//     delete mpSubscriberThread;
//     mpSubscriberThread = 0;
//     delete mpSubscriberComp;
//     mpSubscriberComp = 0;
// }

// void TextUnittestSPSub::testRoutine()
// {
//     int rc = -1;
//     rc = mpSubscriberComp->setNotifications();
//     QVERIFY(-1 != rc);
//     QTest::qWait(500);
//     rc = mpSubscriberComp->requestStart();
//     QVERIFY(-1 != rc);

//     do {
//         QTest::qWait(YaComponent::TimeOut);

//     } while (mpSubscriberComp->miPropertiesCnt <= 12522);

//     QCOMPARE(mpSubscriberComp->miPropertiesCnt, 12522 + 1);
// }

#include "PublisherIfcProxy.h"
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
    int rc = -1;
    rc = mData.setNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
    return rc;
}

int SubscriberComp::requestStart()
{
    return mData.requestStartData();
}

int SubscriberComp::requestStop()
{
    return mData.requestStartData();
}

void SubscriberComp::onProperty(int proxyId, const Data &data)
{
    assert(PROXY_DATA == proxyId);
    qDebug() << "received onProperty" << data.counter();
    miPropertiesCnt++;
    mLastData = data;
}
