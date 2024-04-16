#pragma once
#include <QObject>

#include <yacomponent/YaComponent.h>
#include <yacomponent/YaComponentThread.h>

#include "IPublisherIfcProxyHandler.h"
#include "SubscriberCompImpl.h"

class SubscriberComp : public YaComponent::SubscriberCompImpl,
                       public YaComponent::IPublisherIfcProxyHandler
{
public:
    SubscriberComp(void *context);
    virtual ~SubscriberComp() {}

    void onProperty(int, const Data &);

    virtual void init();

    int setNotifications();
    int clearNotifications();

    int requestStart();
    int requestStop();

    int miPropertiesCnt;

private:
    SubscriberComp &self() { return *this; }
};

class TextUnittestMPSub : public QObject
{
    Q_OBJECT
public:
    TextUnittestMPSub(QObject *parent = 0);
    ~TextUnittestMPSub() {}

signals:

private slots:

    void initTestCase();
    void cleanupTestCase();
    void testMPMTSub();

private:
    void testRoutine();

    void *mpContext;
    YaComponentThread *mpSubscriberThread;
    SubscriberComp *mpSubscriberComp;
};
