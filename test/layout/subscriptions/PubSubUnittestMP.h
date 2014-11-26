#ifndef PUBSUBUNITTESTMP_H
#define PUBSUBUNITTESTMP_H

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
  void finishedPub ( int exitCode, QProcess::ExitStatus exitStatus) { qDebug() << "process pub finished"; miExitCodePub = exitCode; QVERIFY( exitStatus != QProcess::CrashExit);}
  void finishedSubFast ( int exitCode, QProcess::ExitStatus exitStatus) { qDebug() << "process subfast finished"; miExitCodeSubFast = exitCode; QVERIFY( exitStatus != QProcess::CrashExit);}
  void finishedSubSlow ( int exitCode, QProcess::ExitStatus exitStatus) { qDebug() << "process subslow finished"; miExitCodeSubSlow = exitCode; QVERIFY( exitStatus != QProcess::CrashExit);}


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

    int miExitCodePub;
    int miExitCodeSubFast;
    int miExitCodeSubSlow;

};

#endif // TEXTUNITTEST_H
