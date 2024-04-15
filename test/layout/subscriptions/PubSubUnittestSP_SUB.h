#pragma once

#include "IPublisherIfcProxyHandler.h"
#include "SubscriberCompImpl.h"

class SubscriberComp : public YaComponent::SubscriberCompImpl, public IPublisherIfcProxyHandler
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

    Data mLastData;

private:
    SubscriberComp &self() { return *this; }
};
