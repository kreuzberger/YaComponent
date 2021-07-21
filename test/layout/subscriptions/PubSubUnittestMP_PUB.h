#ifndef PUBSUBUNITTESTMP_PUB_H
#define PUBSUBUNITTESTMP_PUB_H

#include <QObject>

#include "YaComponent.h"
#include "YaComponentThread.h"
#include "publishercompunittest.h"

class PubSubUnittestMPPub : public QObject
{
    Q_OBJECT
public:
    PubSubUnittestMPPub(QObject *parent = 0);
    ~PubSubUnittestMPPub() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testMPMTPub();

private:
    void initComponentsMPMT();
    void cleanupComponents();
    void testRoutine();

    void *mpContext;
    YaComponentThread *mpPublisherThread;
    PublisherComp *mpPublisher;
};

#endif // TEXTUNITTEST_H
