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

void TextUnittestMPSub::initComponentsMPMT()
{
    mpContext = YaComponent::context_new();
    mpSubscriberThread = new YaComponentThread();
    mpSubscriberComp = new SubscriberComp(mpContext);

    mpSubscriberThread->start();
    mpSubscriberComp->init();
    mpSubscriberComp->setConnectionParaData("tcp://127.0.0.1:42163", "subscriber_slow");
    mpSubscriberComp->moveToThread(mpSubscriberThread);
}

void TextUnittestMPSub::cleanupTestCase() {}

void TextUnittestMPSub::cleanupComponents()
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
    initComponentsMPMT();

    testRoutine();

    cleanupComponents();
}

void TextUnittestMPSub::testRoutine()
{
    int rc = mpSubscriberComp->setNotifications();
    QVERIFY(-1 != rc);
    QTest::qWait(500);
    rc = mpSubscriberComp->requestStart();
    QVERIFY(-1 != rc);

    do {
        QTest::qWait(YaComponent::TimeOut);

    } while (mpSubscriberComp->miPropertiesCnt <= 12522);

    QCOMPARE(mpSubscriberComp->miPropertiesCnt, 12522 + 1);
}
