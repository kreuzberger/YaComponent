#include "YaStubBase.h"

YaStubBase::YaStubBase(void* context, int id, QObject *parent)
  : QObject(parent)
  , mPublisher( context )
  , mId( id )
{
}

void YaStubBase::send(int key, int size, const char* msg )
{
  mPublisher.send(key, size, msg );
}

void YaStubBase::setConnectionPara( const char* pub, const char* req, int hwm  )
{
  mPublisher.setConnectionPara( pub, req, hwm );
}

void YaStubBase::close()
{
  mPublisher.close();
}

//int YaStubBase::receive(int& key, int& size, const char* msg )
//{
//  return mPublisher.receive(key, size, msg );
//}
