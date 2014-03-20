#include "YaPUBImpl.h"
#include "YaPUBSUB.h"
#include "zmq.h"
#include <assert.h>

YaPUBImpl::YaPUBImpl(void * context)
  : mpPUBSocket(0)
  , mbBound(false)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpPUBSocket = zmq_socket (context, ZMQ_PUB);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }
}

YaPUBImpl::~YaPUBImpl()
{
  zmq_close( mpPUBSocket );
}

bool YaPUBImpl::bind( const char* address)
{
  assert( 0 != mpPUBSocket && 0 != address );
  if( mpPUBSocket && !mbBound )
  {
    if( 0 == zmq_bind(mpPUBSocket,address) )
    {
      mbBound = true;
    }
    else
    {
      fprintf(stderr, "error: cannot bind to address %s\n",address);
    }
  }
  else
  {
    if( mbBound )
    {
      fprintf(stderr, "error: already bound!\n");
    }
    if( !mpPUBSocket )
    {
      fprintf(stderr, "error: no socket!\n");
    }
  }

  return mbBound;

}

int YaPUBImpl::send(int key, int iDataCnt, const char* pcData )
{
  char cKey[YaComponent::KeySize];
  char cSize[YaComponent::DataSize];
  sprintf(cKey,YaComponent::KeyFmt,key);
  sprintf(cSize,YaComponent::DataFmt,iDataCnt);
  int rc = -1;
  assert( 0 != mpPUBSocket && 0 != pcData );
  if( 0 != mpPUBSocket && 0 != pcData)
  {
    //printf("sending with key %s data %d\n",cKey,iDataCnt);
    rc = zmq_send(mpPUBSocket,cKey, YaComponent::KeySize , ZMQ_SNDMORE);
    assert( YaComponent::KeySize == rc);
    rc = zmq_send(mpPUBSocket, cSize, YaComponent::DataSize , ZMQ_SNDMORE);
    assert( YaComponent::DataSize == rc);
    rc = zmq_send(mpPUBSocket, pcData, iDataCnt,0);
    assert( iDataCnt == rc );
  }

  return rc;
}
