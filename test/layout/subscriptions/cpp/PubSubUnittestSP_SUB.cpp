#include "PubSubUnittestSP_SUB.h"
#include <QtTest>

//#include "PublisherIfcProxy.h"
#include <QtCore/QtDebug>

SubscriberComp::SubscriberComp(void *context)
    : IPublisherIfcProxyHandler(*this)
    , YaComponent::SubscriberCompImpl(context, static_cast<IPublisherIfcProxyHandler &>(*this))
    , miPropertiesCnt(0)
    , miPropertiesTimeCnt(0)
{
    //  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
}

void SubscriberComp::init()
{
    SubscriberCompImpl::init();
}

int SubscriberComp::setNotifications()
{
    int rc = -1;
    rc = mData.setNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
    if (-1 == rc)
        return rc;
    rc = mData.setNotification(YaComponent::PublisherIfcProxy::PROP_TIME);
    return rc;
}

int SubscriberComp::requestStart()
{
    return mData.requestStartData();
}

int SubscriberComp::requestStop()
{
    return mData.requestStartData();
}

void SubscriberComp::onProperty(int proxyId, const Data &data)
{
    assert(PROXY_DATA == proxyId);
    qDebug() << "received onProperty" << data.counter();
    miPropertiesCnt++;
    mLastData = data;
}
void SubscriberComp::onProperty(int proxyId, const Time &time)
{
    assert(PROXY_DATA == proxyId);
    qDebug() << "received onProperty Time" << time.counter();
    miPropertiesTimeCnt++;
    mLastTime = time;
}
