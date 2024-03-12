#include "PubSubUnittestMP.h"

QTEST_MAIN(PubSubUnittestMP);

PubSubUnittestMP::PubSubUnittestMP(QObject *parent)
    : QObject(parent)
{}

void PubSubUnittestMP::cleanupTestCase()
{
    moProcessPublisher.kill();
    moProcessSubscriberFast.kill();
    moProcessSubscriberSlow.kill();
}

void PubSubUnittestMP::init()
{
    miExitCodePub = -1;
    miExitCodeSubFast = -1;
    miExitCodeSubSlow = -1;
}

void PubSubUnittestMP::cleanup()
{
    miExitCodePub = -1;
    miExitCodeSubFast = -1;
    miExitCodeSubSlow = -1;
}

void PubSubUnittestMP::testMPMT()
{
    QString publisher = "./publisherMPMT";
    QString subscriber_fast = "./subscriber_fast_MPMT";
    QString subscriber_slow = "./subscriber_slow_MPMT";
#ifdef Q_OS_WIN
    publisher = "publisherMPMT";
    subscriber_fast = "subscriber_fast_MPMT";
    subscriber_slow = "subscriber_slow_MPMT";
#endif
    connect(&moProcessPublisher,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            this,
            SLOT(finishedPub(int, QProcess::ExitStatus)));
    connect(&moProcessSubscriberFast,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            this,
            SLOT(finishedSubFast(int, QProcess::ExitStatus)));
    connect(&moProcessSubscriberSlow,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            this,
            SLOT(finishedSubSlow(int, QProcess::ExitStatus)));

    moProcessPublisher.setProcessChannelMode(QProcess::MergedChannels);

    moProcessPublisher.start(publisher, QStringList() << "-v1");
    QVERIFY(moProcessPublisher.waitForStarted());
    QTest::qSleep(1000);

    moProcessSubscriberFast.start(subscriber_fast, QStringList() << "-v2");
    moProcessSubscriberSlow.start(subscriber_slow, QStringList() << "-v2");
    QVERIFY(moProcessSubscriberFast.waitForStarted());
    QVERIFY(moProcessSubscriberSlow.waitForStarted());

    do {
        QTest::qWait(500);

        fprintf(stderr, "%s", moProcessPublisher.readAll().constData());
        fprintf(stderr, "%s", moProcessSubscriberFast.readAll().constData());
        fprintf(stdout, "%s", moProcessSubscriberSlow.readAll().constData());
    } while (!(-1 != miExitCodePub && -1 != miExitCodeSubFast && -1 != miExitCodeSubSlow));

    fprintf(stderr, "%s\n", moProcessPublisher.readAll().constData());
    fprintf(stderr, "%s\n", moProcessSubscriberFast.readAll().constData());
    fprintf(stderr, "%s\n", moProcessSubscriberSlow.readAll().constData());

    QCOMPARE(miExitCodePub, 0);
    QCOMPARE(miExitCodeSubFast, 0);
    QCOMPARE(miExitCodeSubSlow, 0);
}
