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
    explicit SubscriberComp(void *context);
    virtual ~SubscriberComp() {}

    void onProperty(int, const Data &);
    void onProperty(int, const Time &);

    virtual void init();

    int setNotifications();
    int clearNotifications();

    int requestStart();
    int requestStop();

    int miPropertiesCnt;

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