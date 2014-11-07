#include "YaPUBImpl.h"
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

int YaPUBImpl::receive(int& key, int size, const  char* pcData )
{
  int iBytes = 0;

  if( 0 != mpReqRespSocket )
  {
    // receive key
    iBytes = zmq_recv (mpReqRespSocket, mcKeyReq, YaComponent::KeySize, 0);
    assert (iBytes == YaComponent::KeySize);
    sscanf(mcKeyReq,YaComponent::KeyFmt,&key);
    {
      iBytes = zmq_recv (mpReqRespSocket, mMsgBufferReq.data(), YaComponent::MaxMessageSize, 0);
      if( 0 < iBytes )
      {
        size = iBytes;
        pcData = mMsgBufferReq.data();
      }
    }
  }

  return iBytes;
}

void YaPUBImpl::setConnectionPara(const char* pub, const char* req, int hwm)
{
  assert( 0 != mpPUBSocket && 0 != pub );
  assert( 0 != mpReqRespSocket && 0 != req );
  if( mpPUBSocket && mpReqRespSocket && !mbBound )
  {
    if( 0 < hwm )
    {
      zmq_setsockopt(mpPUBSocket,ZMQ_SNDHWM,&hwm,sizeof(hwm));
      int wait = 1;
      zmq_setsockopt(mpPUBSocket,ZMQ_XPUB_NODROP,&wait,sizeof(wait));
      int timeout = 0;
      zmq_setsockopt (mpPUBSocket, ZMQ_SNDTIMEO, &timeout, 4);
    }

    if( 0 == zmq_bind(mpPUBSocket,pub) && 0 == zmq_bind(mpReqRespSocket,req))
    {
      mbBound = true;
    }
    else
    {
      fprintf(stderr, "error: cannot bind to address %s or syncaddress %s\n",pub,req);
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

 // return mbBound;

}

int YaPUBImpl::send(int key, const ::google::protobuf::MessageLite& msg)
{
  int rc = -1;
  int iSize = msg.GetCachedSize();
  msg.SerializeToArray(mMsgBuffer.data(),iSize);
  rc = send(key, iSize, mMsgBuffer.data());

  return rc;
}


int YaPUBImpl::send(int key, int msgSize, const char* msgData)
{
  sprintf(mcKey,YaComponent::KeyFmt,key);
  int rc = -1;
  assert( 0 != mpPUBSocket  );
  if( 0 != mpPUBSocket)
  {
    rc = zmq_send(mpPUBSocket,mcKey, YaComponent::KeySize , ZMQ_SNDMORE);
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

