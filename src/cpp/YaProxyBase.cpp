#include "YaProxyBase.h"

YaProxyBase::YaProxyBase(void* context, int id, QObject *parent)
  : QObject(parent)
  , mSubscriber( context )
  , mId( id )
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


void YaProxyBase::setConnectionPara(const char * address, const char* ident)
{
  mSubscriber.setConnectionPara(address, ident);
}

void YaProxyBase::close()
{
  mSubscriber.close();
}

