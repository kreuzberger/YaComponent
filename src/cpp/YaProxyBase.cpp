#include "YaProxyBase.h"

YaProxyBase::YaProxyBase(void* context, QObject *parent)
  : QObject(parent)
  , mSubscriber( context )
{
}


void YaProxyBase::setNotification( int key  )
{
  mSubscriber.setNotification( key );
}

void YaProxyBase::clearNotification( int key )
{
  mSubscriber.clearNotification( key );
}

//int YaProxyBase::receive( int& key, int& size, const char* data)
//{
//  return mSubscriber.receive(key, size, data);
//}


void YaProxyBase::setConnectionPara(const char * sub, const char *req, int)
{
  mSubscriber.setConnectionPara(sub,req);
}

