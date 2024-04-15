#include "PubSubUnittestSP_PUB.h"
#include "zmq.h"
#include <QtTest>

// void PubSubUnittestSPPub::initComponentsMPMT()
// {
//     mpContext = YaComponent::context_new();
//     mpPublisher = new PublisherComp(mpContext);
//     mpPublisherThread = new YaComponentThread();

//     mpPublisherThread->start();

//     mpPublisher->init();
//     mpPublisher->setConnectionParaReceiverData("tcp://*:42163", 5000);
//     mpPublisher->miMaxMessageCnt = 12522;
//     mpPublisher->moveToThread(mpPublisherThread);
// }

// void PubSubUnittestSPPub::cleanupComponents()
// {
//     mpPublisher->close();

//     zmq_ctx_term(mpContext);

//     if (mpPublisherThread) {
//         mpPublisherThread->quit();
//         QTest::qWait(100);
//         QVERIFY(mpPublisherThread->isFinished());
//     }

//     mpContext = 0;
//     delete mpPublisherThread;
//     mpPublisherThread = 0;
//     delete mpPublisher;
//     mpPublisher = 0;
// }

// void PubSubUnittestSPPub::testRoutine()
// {
//     QVERIFY(0 == mpPublisher->miRequestStop);
//     do {
//         QTest::qWait(10);

//     } while (!mpPublisher->mbFinished);

//     QTest::qWait(1000);
// }

#include <QtCore/QtDebug>

PublisherComp::PublisherComp(void *context)
    : YaComponent::PublisherCompImpl(context, static_cast<IPublisherIfcStubHandler &>(self()))
    , IPublisherIfcStubHandler(self())
    , miRequestStop(0)
    , miTimerID(-1)
    , miMessageCnt(0)
    , miMaxMessageCnt(1)
    , moData()
    , mbFinished(false)
{}

void PublisherComp::init()
{
    PublisherCompImpl::init();
    moData.mutable_samples()->Reserve(4096);
}

void PublisherComp::onRequestStartData(int id)
{
    qDebug() << "received onRequestStartData, start sending";
    // if (0 < miTimerID) {
    //     qDebug() << "already started";
    // } else {
    //     miMessageCnt = 0;
    //     timerEvent(0);
    //     miTimerID = QObject::startTimer(20);
    //     mbFinished = false;
    // }
}

void PublisherComp::onRequestStopData(int id)
{
    qDebug() << "received onRequestStopData";
    // miRequestStop++;
    // if (0 < miTimerID) {
    //     QObject::killTimer(miTimerID);
    // }
    // miTimerID = 0;
    // qDebug() << "send %d messages" << miMessageCnt;
}

int PublisherComp::sendData(const Data &data)
{
    moData = data;
    return mReceiverData.send(YaComponent::PublisherIfcStub::PROP_DATA, data);
}
// void PublisherComp::timerEvent(QTimerEvent *)
// {
//     int rc = -1;

//     do {
//         //moText.set_text(qPrintable(oText.arg(miMessageCnt)));
//         moData.set_timeseconds(0);
//         moData.set_timefraction(0.0);
//         moData.set_counter(miMessageCnt);
//         for (int idx = 0; idx < 2048; idx++) {
//             //moData.set_samples(idx, rand() / std::numeric_limits<int>::max() );
//         }

//         rc = mReceiverData.send(YaComponent::PublisherIfcStub::PROP_DATA, moData);
//         if (-1 != rc) {
//             miMessageCnt++;
//         }
//         if (miMessageCnt >= miMaxMessageCnt) {
//             if (0 < miTimerID) {
//                 QObject::killTimer(miTimerID);
//                 miTimerID = 0;
//             }
//             mbFinished = true;
//             break;
//         }
//     } while (rc != -1);
// }
