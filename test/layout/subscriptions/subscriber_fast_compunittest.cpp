#include "subscriber_fast_compunittest.h"
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
  int rc = -1;
  rc = mData.setNotification(YaComponent::PublisherIfcProxy::PROP_DATA);
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


void SubscriberComp::onProperty( int proxyId, const Data& data)
{
  assert( PROXY_DATA == proxyId);
  miPropertiesCnt++;
  if( miPropertiesCnt % 100 == 0)
  {
    qDebug() << "fast received " << miPropertiesCnt ;
  }

}

