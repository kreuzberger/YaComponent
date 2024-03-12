#pragma once
#include <QObject>

#include <yacomponent/YaComponent.h>
#include <yacomponent/YaComponentThread.h>
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
