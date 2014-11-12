#include "YaSUBImpl.h"

#include "zmq.h"
#include <QtCore/QCoreApplication>



#include <assert.h>
#include <memory.h>

YaSUBImpl::YaSUBImpl(void * context)
  : mpSUBSocket(0)
  , mpReqRespSocket(0)
  , mbConnected(false)
  , mMsgOutBuffer(YaComponent::MessageSize)
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
}

int YaSUBImpl::setNotification(int key)
{
  char cKey[YaComponent::KeySize];
  sprintf(cKey,YaComponent::KeyFmt,key);

  zmq_setsockopt (mpSUBSocket, ZMQ_SUBSCRIBE, cKey, YaComponent::KeySize);
}

int YaSUBImpl::request(int key, const ::google::protobuf::MessageLite& msg )
{
  int rc = -1;
  int iSize = msg.GetCachedSize();
  msg.SerializeToArray(mMsgOutBuffer.data(),iSize);
  rc = send( key, iSize, mMsgOutBuffer.data());
  return rc;
}


int YaSUBImpl::send( int key, int size, const char* data )
{
  sprintf(mcKey,YaComponent::KeyFmt,key);
  int rc = -1;
  assert( 0 != mpReqRespSocket  );
  int flags = 0;
  if( 0 != size && 0 != data ) flags = ZMQ_SNDMORE;
  if( 0 != mpReqRespSocket)
  {
    rc = zmq_send(mpReqRespSocket,mcKey, YaComponent::KeySize , flags);
    assert( -1 != rc);
    if( 0 != size && 0 != data )
    {
      rc = zmq_send (mpReqRespSocket, data, size, 0);
    }
  }

  return rc;
}

int YaSUBImpl::clearNotification(int key)
{
  char cKey[YaComponent::KeySize];
  sprintf(cKey,YaComponent::KeyFmt,key);

  zmq_setsockopt (mpSUBSocket, ZMQ_UNSUBSCRIBE, cKey, YaComponent::KeySize);
}

void YaSUBImpl::setConnectionPara( const char* address, const char* syncaddress, int )
{
  assert( 0 != mpSUBSocket && 0 != address );
  assert( 0 != mpReqRespSocket && 0 != syncaddress );
  if( mpSUBSocket && mpReqRespSocket && !mbConnected )
  {
    if( 0 == zmq_connect(mpSUBSocket,address) && ( 0 == zmq_connect(mpReqRespSocket,syncaddress)))
    {
      mbConnected = true;
      char cKey[YaComponent::KeySize];
      sprintf(cKey,YaComponent::KeyFmt,YaComponent::KeySync);
      // this is a blocking call!
      int rc = zmq_send (mpReqRespSocket, cKey, YaComponent::KeySize, 0);
      assert( -1 != rc);
      YaComponent::sleep(20);
      bool bSync = false;
      do
      {
        QCoreApplication::processEvents();
        bSync = checkSync();
      } while( !bSync);
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

  //return mbConnected;

}

bool YaSUBImpl::checkSync()
{
  bool bRet = false;
  int size = zmq_recv (mpReqRespSocket, mMsgBuffer.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
  if ( 0 < size )
  {
    bRet = ( 0 == strncmp(YaComponent::SynAck,mMsgBuffer.data(),strlen(YaComponent::SynAck))) ? true : false;
  }

  return bRet;
}

int YaSUBImpl::receive(int& key, int& size, const  char* pcData )
{
  int iBytes = 0;

  if( 0 != mpSUBSocket )
  {
    // receive key
    iBytes = zmq_recv (mpSUBSocket, mcKey, YaComponent::KeySize, ZMQ_NOBLOCK);
    if( 0 < iBytes )
    {
      assert (iBytes == YaComponent::KeySize);
      sscanf(mcKey,YaComponent::KeyFmt,&key);
      {
        iBytes = zmq_recv (mpSUBSocket, mMsgBuffer.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
        if( 0 < iBytes )
        {
          size = iBytes;
          pcData = mMsgBuffer.data();
        }
      }
      miMessageCnt++;
    }
  }

  return iBytes;
}
