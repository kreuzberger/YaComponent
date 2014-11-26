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

void YaStubBase::setConnectionPara( const char* address,  int hwm  )
{
  mPublisher.setConnectionPara( address, hwm );
}

void YaStubBase::close()
{
  mPublisher.close();
}

