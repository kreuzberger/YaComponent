#include "TextUnittestMP_PUB.h"
#include "zmq.h"
#include <QtTest>

QTEST_MAIN(TextUnittestMPPub);

TextUnittestMPPub::TextUnittestMPPub(QObject *parent)
    : QObject(parent)
    , mpContext(0)
    , mpTextGenThread(0)
    , mpTextGen1(0)
{}

void TextUnittestMPPub::initComponentsMPMT()
{
    mpContext = YaComponent::context_new();
    mpTextGen1 = new TextGenComp(mpContext);
    mpTextGenThread = new YaComponentThread();

    mpTextGenThread->start();

    mpTextGen1->init();
    mpTextGen1->setConnectionParaTextGenXml("tcp://*:42163", 5000);
    mpTextGen1->setConnectionParaTextGenPlain("tcp://*:42263", 5000);
    mpTextGen1->moveToThread(mpTextGenThread);
}
void TextUnittestMPPub::cleanupTestCase() {}

void TextUnittestMPPub::cleanupComponents()
{
    mpTextGen1->close();

    zmq_ctx_term(mpContext);

    if (mpTextGenThread) {
        mpTextGenThread->quit();
        QTest::qWait(100);
        QVERIFY(mpTextGenThread->isFinished());
    }

    mpContext = 0;
    delete mpTextGenThread;
    mpTextGenThread = 0;
    delete mpTextGen1;
    mpTextGen1 = 0;
}

void TextUnittestMPPub::testMPMTPub()
{
    initComponentsMPMT();

    testRoutine();
    cleanupComponents();
}

void TextUnittestMPPub::testRoutine()
{
    QVERIFY(0 == mpTextGen1->miRequestStop);
    for (int idx = 0; idx < 10; idx++) {
        QTest::qWait(500);
        if (1 == mpTextGen1->miRequestStop) {
            break;
        }
    }
    QVERIFY(1 == mpTextGen1->miRequestStop);
    QTest::qWait(1000);
}
