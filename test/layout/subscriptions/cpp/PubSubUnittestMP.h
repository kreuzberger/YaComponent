#pragma once
#include <QObject>

#include <QtCore/QProcess>
#include <QtTest>
class PubSubUnittestMP : public QObject
{
    Q_OBJECT
public:
    PubSubUnittestMP(QObject *parent = 0);
    ~PubSubUnittestMP() {}

signals:

public slots:
    void finishedPub(int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << "process pub finished with" << exitCode;
        miExitCodePub = exitCode;
        QVERIFY(exitStatus != QProcess::CrashExit);
    }
    void finishedSubFast(int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << "process subfast finished with" << exitCode;
        miExitCodeSubFast = exitCode;
        QVERIFY(exitStatus != QProcess::CrashExit);
    }
    void finishedSubSlow(int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << "process subslow finished with" << exitCode;
        miExitCodeSubSlow = exitCode;
        QVERIFY(exitStatus != QProcess::CrashExit);
    }

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void cleanup();
    void init();
    void testMPMT();

private:
    QProcess moProcessPublisher;
    QProcess moProcessSubscriberFast;
    QProcess moProcessSubscriberSlow;

    int miExitCodePub = 0;
    int miExitCodeSubFast = 0;
    int miExitCodeSubSlow = 0;
};
