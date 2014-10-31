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
  char buffer [256];
  int size = zmq_recv (mpReqRespSocket, buffer, 255, 0);
  if (size == -1);
  if (size > 255)
      size = 255;
  buffer[size] = 0;

  return (0 != strlen(buffer));
}

int YaSUBImpl::receive(int& key, const  char* pcData )
{
  int rc = -1;
  int iBytes = 0;
  int iDataSize = 0;
  assert( 0 != mpSUBSocket /*&& 0 != pcData*/ );
  if( 0 != mpSUBSocket /*&& 0 != pcData*/)
  {
    // receive key
    iBytes = zmq_recv (mpSUBSocket, mpcKey, YaComponent::KeySize, 0);
    assert (iBytes == YaComponent::KeySize);
    sscanf(mpcKey,YaComponent::KeyFmt,&key);
    // receive data size
//    iBytes = zmq_recv (mpSUBSocket, mpcMessageSize, YaComponent::MessageSize, 0);
//    assert (iBytes == YaComponent::MessageSize);
//    sscanf(mpcMessageSize,YaComponent::MessageSizeFmt, &iDataSize);
//    // receive message
//    assert( iDataSize <= YaComponent::MaxMessageSize );
//    if(0 < iDataSize)
    {
      iBytes = zmq_recv (mpSUBSocket, mMsgBuffer.data(), YaComponent::MaxMessageSize, 0);
//      Text txtRcv;
      if( 0 < iBytes )
      {
//        txtRcv.ParseFromArray(mpcMessage,iBytes);
//        txtRcv.PrintDebugString();
      }

    //  if( iBytes != iDataSize  )
    //  assert (iBytes == iDataSize);
    }
    miMessageCnt++;
  }

  return iBytes;
}
