#pragma once
#include <QObject>

#include "IPublisherIfcStubHandler.h"
#include "PublisherCompImpl.h"
#include <yacomponent/YaComponent.h>
#include <QtCore/QThread>

class PublisherComp : public YaComponent::PublisherCompImpl, public YaComponent::IPublisherIfcStubHandler
{
public:
    PublisherComp(void *context);
    virtual ~PublisherComp() {}

    virtual void onRequestStartData(int id);
    virtual void onRequestStopData(int id);
    virtual void init();
    int miTimerID;
    int mConsumers = 0;
    bool mbFinished;

    void sendResponseStop();

protected:
    virtual void timerEvent(QTimerEvent *);

private:
    Data moData;
};

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
    QThread *mpPublisherThread;
    PublisherComp *mpPublisher;
};
