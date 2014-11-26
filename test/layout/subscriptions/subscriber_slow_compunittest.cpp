#include "subscriber_slow_compunittest.h"
#include "PublisherIfcProxy.h"
#include <QtCore/QtDebug>

SubscriberComp::SubscriberComp( void* context )
 : IPublisherIfcProxyHandler( self() )
, YaComponent::SubscriberCompImpl( context, static_cast<IPublisherIfcProxyHandler&>(self()) )
, miPropertiesCnt(0)
{
//  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);

}

void SubscriberComp::init()
{
  SubscriberCompImpl::init();

}

int SubscriberComp::setNotifications()
{
  return mData.setNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
}

int SubscriberComp::requestStart()
{
  return mData.requestStartData();
}

int SubscriberComp::requestStop()
{
  return mData.requestStopData();
}


void SubscriberComp::onProperty( int proxyId, const Data& data)
{
  assert( PROXY_DATA == proxyId);
  miPropertiesCnt++;
  if( miPropertiesCnt % 100 == 0)
  {
    qDebug() << "slow received "<< miPropertiesCnt;
    YaComponent::sleep(50);

  }

}

