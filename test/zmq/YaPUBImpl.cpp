#include "YaPUBImpl.h"
#include "YaComponent.h"
#include "zmq.h"
#include <string.h>
#include <assert.h>
#include <malloc.h>

YaPUBImpl::YaPUBImpl(void * context)
  : mpPUBSocket(0)
  , mpPUBSyncSocket(0)
  , mbBound(false)
  , miSubscribersCnt(0)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpPUBSocket = zmq_socket (context, ZMQ_PUB);
    mpPUBSyncSocket = zmq_socket (context, ZMQ_REP);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }
}

YaPUBImpl::~YaPUBImpl()
{
  zmq_close( mpPUBSocket );
  zmq_close( mpPUBSyncSocket );
}

void YaPUBImpl::close()
{
  zmq_close( mpPUBSocket );
  zmq_close( mpPUBSyncSocket );
}

bool YaPUBImpl::checkSubscribers(int iNumExpectedSubscribers)
{
  char buffer [256];
  while( miSubscribersCnt < iNumExpectedSubscribers)
  {
    char* str = YaComponent::socket_rcv(mpPUBSyncSocket);
    free(str);
      //  s_send (syncservice, "");
    YaComponent::socket_snd(mpPUBSyncSocket, "SYNC_ACK");
    miSubscribersCnt++;
  }

  return (miSubscribersCnt == iNumExpectedSubscribers);
}

bool YaPUBImpl::bind( const char* address, const char* syncaddress)
{
  assert( 0 != mpPUBSocket && 0 != address );
  assert( 0 != mpPUBSyncSocket && 0 != syncaddress );
  if( mpPUBSocket && mpPUBSyncSocket && !mbBound )
  {
    if( 0 == zmq_bind(mpPUBSocket,address) && 0 == zmq_bind(mpPUBSyncSocket,syncaddress))
    {
      mbBound = true;
    }
    else
    {
      fprintf(stderr, "error: cannot bind to address %s or syncaddress %s\n",address,syncaddress);
    }
  }
  else
  {
    if( mbBound )
    {
      fprintf(stderr, "error: already bound!\n");
    }
    if( !mpPUBSocket || !mpPUBSyncSocket)
    {
      fprintf(stderr, "error: no socket!\n");
    }
  }

  return mbBound;

}

int YaPUBImpl::send(int key, int iDataCnt, const char* pcData )
{
  char cKey[YaComponent::KeySize];
  char cSize[YaComponent::MessageSize];
  sprintf(cKey,YaComponent::KeyFmt,key);
  sprintf(cSize,YaComponent::MessageSizeFmt,iDataCnt);
  int rc = -1;
  assert( 0 != mpPUBSocket  );
  if( 0 != mpPUBSocket)
  {
    //printf("sending with key %s data %d\n",cKey,iDataCnt);
    rc = zmq_send(mpPUBSocket,cKey, YaComponent::KeySize , ZMQ_SNDMORE);
    assert( YaComponent::KeySize == rc);
    if( 0 < iDataCnt && 0 != pcData )
    {
//      rc = zmq_send(mpPUBSocket, cSize, YaComponent::MessageSize , ZMQ_SNDMORE);
//      assert( YaComponent::MessageSize == rc);
      rc = zmq_send(mpPUBSocket, pcData, iDataCnt,0);
      assert( iDataCnt == rc );
    }
    else
    {
      rc = zmq_send(mpPUBSocket, cSize, YaComponent::MessageSize , 0);
    }
  }


  return rc;
}
