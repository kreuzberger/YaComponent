#include "YaPUBImpl.h"
#include "YaComponent.h"
#include "zmq.h"
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>

YaPUBImpl::YaPUBImpl(void * context)
  : mpPUBSocket(0)
  , mpReqRespSocket(0)
  , mbBound(false)
  , miSubscribersCnt(0)
  , mMsgBuffer(YaComponent::MaxMessageSize)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpPUBSocket = zmq_socket (context, ZMQ_PUB);
    mpReqRespSocket = zmq_socket (context, ZMQ_REP);
    int hwm = 5000;
    zmq_setsockopt(mpPUBSocket,ZMQ_SNDHWM,&hwm,sizeof(hwm));
    int wait = 1;
    zmq_setsockopt(mpPUBSocket,ZMQ_XPUB_NODROP,&wait,sizeof(wait));
    int timeout = 0;
    zmq_setsockopt (mpPUBSocket, ZMQ_SNDTIMEO, &timeout, 4);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }
}

YaPUBImpl::~YaPUBImpl()
{
  zmq_close( mpPUBSocket );
  zmq_close( mpReqRespSocket );
}

void YaPUBImpl::close()
{
  zmq_close( mpPUBSocket );
  zmq_close( mpReqRespSocket );
}

bool YaPUBImpl::checkSubscribers(int iNumExpectedSubscribers)
{
  char buffer [256];
  while( miSubscribersCnt < iNumExpectedSubscribers)
  {
    char* str = YaComponent::socket_rcv(mpReqRespSocket);
    free(str);
      //  s_send (syncservice, "");
    YaComponent::socket_snd(mpReqRespSocket, "SYNC_ACK");
    miSubscribersCnt++;
  }

  return (miSubscribersCnt == iNumExpectedSubscribers);
}

bool YaPUBImpl::bind( const char* address, const char* syncaddress)
{
  assert( 0 != mpPUBSocket && 0 != address );
  assert( 0 != mpReqRespSocket && 0 != syncaddress );
  if( mpPUBSocket && mpReqRespSocket && !mbBound )
  {
    if( 0 == zmq_bind(mpPUBSocket,address) && 0 == zmq_bind(mpReqRespSocket,syncaddress))
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
    if( !mpPUBSocket || !mpReqRespSocket)
    {
      fprintf(stderr, "error: no socket!\n");
    }
  }

  return mbBound;

}

int YaPUBImpl::send(int key, const ::google::protobuf::MessageLite* msg)
{
  int rc = -1;
  int iSize = 0;
  if( msg )
  {
    iSize = msg->GetCachedSize();
    msg->SerializeToArray(mMsgBuffer.data(),iSize);
  }
  rc = send(key, iSize, mMsgBuffer.data());

  return rc;
}


int YaPUBImpl::send(int key, int msgSize, const char* msgData)
{
  char cKey[YaComponent::KeySize];
  char cSize[YaComponent::MessageSize];
  sprintf(cKey,YaComponent::KeyFmt,key);
  int rc = -1;
  assert( 0 != mpPUBSocket  );
  if( 0 != mpPUBSocket)
  {
    rc = zmq_send(mpPUBSocket,cKey, YaComponent::KeySize , ZMQ_SNDMORE);
    if( -1 != rc )
    {
      if( 0 != msgSize && 0 != msgData )
      {
        rc = zmq_send(mpPUBSocket, msgData, msgSize,0);
      }
      else
      {
        rc = zmq_send(mpPUBSocket, 0, 0,0);
      }
    }
  }

  if( -1 == rc )
  {
    assert( errno == EAGAIN );
  }

  return rc;
}

