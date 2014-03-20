#include "YaSUBImpl.h"
#include "YaPUBSUB.h"
#include "zmq.h"


#include <assert.h>
#include <memory.h>

YaSUBImpl::YaSUBImpl(void * context)
  : mpSUBSocket(0)
  , mbConnected(false)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpSUBSocket = zmq_socket (context, ZMQ_SUB);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }
}

YaSUBImpl::~YaSUBImpl()
{
  zmq_close( mpSUBSocket );
}

int YaSUBImpl::setNotification(int key)
{
  char cKey[YaComponent::KeySize];
  sprintf(cKey,YaComponent::KeyFmt,key);

  zmq_setsockopt (mpSUBSocket, ZMQ_SUBSCRIBE, cKey, YaComponent::KeySize);
}

bool YaSUBImpl::connect( const char* address)
{
  assert( 0 != mpSUBSocket && 0 != address );
  if( mpSUBSocket && !mbConnected )
  {
    if( 0 == zmq_connect(mpSUBSocket,address) )
    {
      mbConnected = true;
    }
    else
    {
      fprintf(stderr, "error: cannot connect to address %s\n",address);
    }
  }
  else
  {
    if( mbConnected )
    {
      fprintf(stderr, "error: already connected!\n");
    }
    if( !mpSUBSocket )
    {
      fprintf(stderr, "error: no socket!\n");
    }
  }

  return mbConnected;

}

int YaSUBImpl::receive(const  char* pcData )
{
  char cKey[YaComponent::KeySize];
  char cSize[YaComponent::DataSize];
  int rc = -1;
  int iBytes = 0;
  int iDataSize = 0;
  assert( 0 != mpSUBSocket /*&& 0 != pcData*/ );
  if( 0 != mpSUBSocket /*&& 0 != pcData*/)
  {
    // receive key
    iBytes = zmq_recv (mpSUBSocket, cKey, YaComponent::KeySize, 0);
    assert (iBytes == YaComponent::KeySize);
    // receive data size
    iBytes = zmq_recv (mpSUBSocket, cSize, YaComponent::DataSize, 0);
    assert (iBytes == YaComponent::DataSize);
    sscanf(cSize,YaComponent::DataFmt, &iDataSize);
    // receive message
    char buf [iDataSize];
    memset(buf,0,iDataSize);
    iBytes = zmq_recv (mpSUBSocket, buf, iDataSize, 0);
    if( iBytes != iDataSize  )
    assert (iBytes == iDataSize);
  }

  return iBytes;
}
