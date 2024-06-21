#pragma once

#include "IPublisherIfcProxyHandler.h"
#include "SubscriberCompImpl.h"

class SubscriberComp : public YaComponent::SubscriberCompImpl,
                       public YaComponent::IPublisherIfcProxyHandler
{
public:
    SubscriberComp(void *context);
    virtual ~SubscriberComp() {}

    void onProperty(int, const Data &);
    void onProperty(int, const Time &);

    virtual void init();

    int setNotifications();
    int clearNotifications();

    int requestStart();
    int requestStop();

    int miPropertiesCnt;
    int miPropertiesTimeCnt;

    Data mLastData;
    Time mLastTime;

private:
};
