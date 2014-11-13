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
  , mMsgPropBuffer(YaComponent::MaxMessageSize)
  , mMsgRespBuffer(YaComponent::MaxMessageSize)
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
  memset(mcIdent,0,YaComponent::MaxIdentSize);

}

void YaSUBImpl::close()
{
//  send( YaComponent::KeyEnd, 0, 0);
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
  char cKey[YaComponent::KeySize + 1];
  sprintf(cKey,YaComponent::KeyFmt,key);

  zmq_setsockopt (mpSUBSocket, ZMQ_SUBSCRIBE, cKey, YaComponent::KeySize);
}

int YaSUBImpl::request(int key, const ::google::protobuf::MessageLite& msg )
{
  int rc = -1;
  int iSize = msg.ByteSize();
  msg.SerializeToArray(mMsgOutBuffer.data(),iSize);
  assert( iSize == msg.GetCachedSize());
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
//    rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
//    assert( -1 != rc);
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

void YaSUBImpl::setConnectionPara( const char* address, const char* syncaddress, const char* ident )
{
  assert( 0 != mpSUBSocket && 0 != address );
  assert( 0 != mpReqRespSocket && 0 != syncaddress );

  assert(strlen(ident) < YaComponent::MaxIdentSize );
  strcpy(mcIdent,ident);

  if( mpSUBSocket && mpReqRespSocket && !mbConnected )
  {
    int rc = -1;
    zmq_setsockopt( mpReqRespSocket, ZMQ_IDENTITY, mcIdent, strlen(mcIdent));
    if( 0 == zmq_connect(mpSUBSocket,address) && ( 0 == zmq_connect(mpReqRespSocket,syncaddress)))
    {
      mbConnected = true;
      char cKey[YaComponent::KeySize + 1];
      sprintf(cKey,YaComponent::KeyFmt,YaComponent::KeySync);

//      int rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
//      assert( -1 != rc);
      rc = zmq_send (mpReqRespSocket, cKey, YaComponent::KeySize, ZMQ_NOBLOCK);
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
  int size = zmq_recv (mpReqRespSocket, mMsgPropBuffer.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
  if ( 0 < size )
  {
    bRet = ( 0 == strncmp(YaComponent::SynAck,mMsgPropBuffer.data(),strlen(YaComponent::SynAck))) ? true : false;
  }

  return bRet;
}

int YaSUBImpl::receive(int& key, int& size,  char** pcData )
{
  int iBytes = 0;
  char buffer[YaComponent::MaxIdentSize];
  int rc = -1;

  assert( 0 != mpSUBSocket && 0 != mpReqRespSocket);

  if( 0 != mpSUBSocket && 0 != mpReqRespSocket)
  {
    // start receiving responses
    iBytes = zmq_recv (mpReqRespSocket, mcKey, YaComponent::KeySize, ZMQ_NOBLOCK);
    if( 0 < iBytes )
    {
      assert (iBytes == YaComponent::KeySize);
      sscanf(mcKey,YaComponent::KeyFmt,&key);
      if( 0 < key)
      {
        iBytes = zmq_recv (mpReqRespSocket, mMsgRespBuffer.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
        if( 0 < iBytes )
        {
          size = iBytes;
          *pcData = mMsgRespBuffer.data();
        }
      }
      else if ( YaComponent::KeySync == key)
      {
        iBytes = 0;
      }
      miMessageCnt++;
    }
    else // no response received, check for properties
    {
      // receive key
      iBytes = zmq_recv (mpSUBSocket, mcKey, YaComponent::KeySize, ZMQ_NOBLOCK);
      if( 0 < iBytes )
      {
        assert (iBytes == YaComponent::KeySize);
        sscanf(mcKey,YaComponent::KeyFmt,&key);
        {
          iBytes = zmq_recv (mpSUBSocket, mMsgPropBuffer.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
          if( 0 < iBytes )
          {
            size = iBytes;
            *pcData = mMsgPropBuffer.data();
          }
        }
        miMessageCnt++;
      }
    }
  }

  return iBytes;
}
