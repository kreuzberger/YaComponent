#pragma once
#include <QObject>

#include "IPublisherIfcProxyHandler.h"
#include "SubscriberCompImpl.h"
#include <yacomponent/YaComponent.h>
#include <QtCore/QThread>

class SubscriberComp : public YaComponent::SubscriberCompImpl,
                       public YaComponent::IPublisherIfcProxyHandler
{
public:
    explicit SubscriberComp(void *context);
    virtual ~SubscriberComp() {}

    void onPropertyData(int, const Data &) override;
    void onPropertyTime(int, const Time &) override;

    virtual void init() override;

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
    QThread *mpSubscriberThread;
    SubscriberComp *mpSubscriberComp;
};
