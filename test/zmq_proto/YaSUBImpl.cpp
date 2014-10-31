#include "YaSUBImpl.h"
#include "YaComponent.h"
#include "zmq.h"


#include <assert.h>
#include <memory.h>

#include "BasicInterface.pb.h"

YaSUBImpl::YaSUBImpl(void * context)
  : mpSUBSocket(0)
  , mpReqRespSocket(0)
  , mbConnected(false)
  , mMsgSizeBuffer(YaComponent::MessageSize)
  , mMsgBuffer(YaComponent::MaxMessageSize)
  , miMessageCnt(0)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpSUBSocket = zmq_socket (context, ZMQ_SUB);
    YaComponent::sleep(1);
    mpReqRespSocket = zmq_socket (context, ZMQ_REQ);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }

  mpcKey = new char[YaComponent::KeySize];
}

void YaSUBImpl::close()
{
  zmq_close( mpSUBSocket );
  zmq_close( mpReqRespSocket );
}

YaSUBImpl::~YaSUBImpl()
{
  zmq_close( mpSUBSocket );
  zmq_close( mpReqRespSocket );

  delete [] mpcKey;
}

int YaSUBImpl::setNotification(int key)
{
  char cKey[YaComponent::KeySize];
  sprintf(cKey,YaComponent::KeyFmt,key);

  zmq_setsockopt (mpSUBSocket, ZMQ_SUBSCRIBE, cKey, YaComponent::KeySize);
}

bool YaSUBImpl::connect( const char* address, const char* syncaddress)
{
  assert( 0 != mpSUBSocket && 0 != address );
  assert( 0 != mpReqRespSocket && 0 != syncaddress );
  if( mpSUBSocket && mpReqRespSocket && !mbConnected )
  {
    if( 0 == zmq_connect(mpSUBSocket,address) && ( 0 == zmq_connect(mpReqRespSocket,syncaddress)))
    {
      mbConnected = true;
      zmq_send (mpReqRespSocket, "SYNC_SYN", strlen("SYNC_SYN"), 0);
    }
    else
    {
      fprintf(stderr, "error: cannot connect to address %s or syncaddress %s\n",address, syncaddress);
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

bool YaSUBImpl::checkSync()
{
  bool bRet = false;
  int size = zmq_recv (mpReqRespSocket, mMsgBuffer.data(), YaComponent::MaxMessageSize, 0);
  if ( 0 < size )
  {
    bRet = ( 0 == strncmp(YaComponent::SynAck,mMsgBuffer.data(),strlen(YaComponent::SynAck))) ? true : false;
  }

  return bRet;
}

int YaSUBImpl::receive(int& key, const  char* pcData )
{
  int iBytes = 0;

  if( 0 != mpSUBSocket )
  {
    // receive key
    iBytes = zmq_recv (mpSUBSocket, mpcKey, YaComponent::KeySize, 0);
    assert (iBytes == YaComponent::KeySize);
    sscanf(mpcKey,YaComponent::KeyFmt,&key);
    {
      iBytes = zmq_recv (mpSUBSocket, mMsgBuffer.data(), YaComponent::MaxMessageSize, 0);
      if( 0 < iBytes )
      {
      }
    }
    miMessageCnt++;
  }

  return iBytes;
}
