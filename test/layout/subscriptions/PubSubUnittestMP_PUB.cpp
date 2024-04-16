#include "PubSubUnittestMP_PUB.h"
#include <QtTest>

QTEST_MAIN(PubSubUnittestMPPub);

PubSubUnittestMPPub::PubSubUnittestMPPub(QObject *parent)
    : QObject(parent)
    , mpContext(0)
    , mpPublisherThread(0)
    , mpPublisher(0)
{}

void PubSubUnittestMPPub::initComponentsMPMT()
{
    mpContext = YaComponent::context_new();
    mpPublisher = new PublisherComp(mpContext);
    mpPublisherThread = new YaComponentThread();

    mpPublisherThread->start();

    mpPublisher->init();
    mpPublisher->setConnectionParaReceiverData("tcp://*:42163", 5000);
    mpPublisher->moveToThread(mpPublisherThread);
}
void PubSubUnittestMPPub::cleanupTestCase() {}

void PubSubUnittestMPPub::cleanupComponents()
{
    mpPublisher->close();

    YaComponent::context_term(mpContext);

    if (mpPublisherThread) {
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
    do {
        QTest::qWait(YaComponent::TimeOut);
    } while (!mpPublisher->mbFinished);

    qInfo() << "PubSubUnittestMPPub::testRoutine finished";
    QTest::qWait(500);
}

#include <QtCore/QtDebug>

PublisherComp::PublisherComp(void *context)
    : YaComponent::PublisherCompImpl(context, dynamic_cast<IPublisherIfcStubHandler &>(*this))
    , IPublisherIfcStubHandler(*this)
    , miTimerID(-1)
    , moData()
    , mbFinished(false)
{}

void PublisherComp::init()
{
    PublisherCompImpl::init();
    moData.mutable_samples()->Reserve(4096);
    mConsumers = 0;
}

void PublisherComp::onRequestStartData(int id)
{
    mConsumers++;
    assert(mConsumers <= 2);
    if (mConsumers > 2) {
        qFatal("maximum number of consumers greater than expected 2");
    }
    qDebug() << "received onRequestStartData, start sending, consumers currently" << mConsumers;
    if (0 < miTimerID) {
        qDebug() << "already started";
    } else {
        miTimerID = QObject::startTimer(YaComponent::TimeOut);
    }
}

void PublisherComp::onRequestStopData(int id)
{
    mConsumers--;
    qDebug() << "received onRequestStopData, actual consumers" << mConsumers;
    if (0 == mConsumers) {
        qInfo() << "no more consumers, stop and exit";
        mbFinished = true;

        if (0 < miTimerID) {
            QObject::killTimer(miTimerID);
            miTimerID = 0;
        }
    }
}

void PublisherComp::timerEvent(QTimerEvent *)
{
    int rc = -1;
    static int messageCount = 0;

    do {
        //moText.set_text(qPrintable(oText.arg(miMessageCnt)));
        moData.set_timeseconds(0);
        moData.set_timefraction(0.0);
        moData.set_counter(messageCount);
        messageCount++;
        // for (int idx = 0; idx < 2048; idx++) {
        //     //moData.set_samples(idx, rand() / std::numeric_limits<int>::max() );
        // }

        rc = mReceiverData.send(YaComponent::PublisherIfcStub::PROP_DATA, moData);
        messageCount++;
        if (messageCount % 10 == 0) {
            break;
        }

    } while (rc != -1);
}
