#include <yacomponent/YaSUBImpl.h>

#include "zmq.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>

#include <assert.h>
#include <thread>

YaSUBImpl::YaSUBImpl( void* context )
{
  assert( 0 != context );
  if ( 0 != context )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    mpReqRespSocket = zmq_socket( context, ZMQ_DEALER );
  }
  else
  {
    fprintf( stderr, "error: invalid context\n" );
  }
  memset( mcIdent, 0, YaComponent::MaxIdentSize );
}

void YaSUBImpl::close()
{
  if ( mpReqRespSocket )
  {
    zmq_close( mpReqRespSocket );
    mpReqRespSocket = nullptr;
  }
}

YaSUBImpl::~YaSUBImpl()
{
  zmq_close( mpReqRespSocket );
  mpReqRespSocket = nullptr;
}

int YaSUBImpl::setNotification( int key )
{
  int  rc = -1;
  char cKey[YaComponent::KeySize + 1];
  sprintf( cKey, YaComponent::KeyFmt, key );

  rc = send( YaComponent::KeySetNotification, YaComponent::KeySize, cKey );
  return rc;
}

int YaSUBImpl::request( int key, const ::google::protobuf::MessageLite& msg )
{
  int rc    = -1;
  int iSize = msg.ByteSizeLong();
  msg.SerializeToArray( mMsgOutBuffer.data(), iSize );
  assert( iSize == msg.GetCachedSize() );
  rc = send( key, iSize, mMsgOutBuffer.data() );
  return rc;
}

int YaSUBImpl::request( int key )
{
  int rc = -1;
  rc     = send( key, 0, 0 );
  return rc;
}

int YaSUBImpl::send( int key, int size, const char* data )
{
  int rc = -1;
  assert( 0 != mpReqRespSocket );
  if ( 0 != mpReqRespSocket )
  {
    rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
    assert( -1 != rc );
    if ( -1 == rc )
    {
      fprintf( stderr, "YaSUBImpl::send:error: %s\n", zmq_strerror( zmq_errno() ) );
    }

    auto send_more = ( 0 != size && data );
    int  flags     = send_more ? ZMQ_SNDMORE : 0;
    char ckey[YaComponent::KeySize + 1];
    sprintf( ckey, YaComponent::KeyFmt, key );
    // qDebug() << "YaSUBImpl::send: key " << ckey << "from key value" << key;
    rc = zmq_send( mpReqRespSocket, ckey, YaComponent::KeySize, flags );
    assert( -1 != rc );
    if ( send_more )
    {
      rc = zmq_send( mpReqRespSocket, data, size, 0 );
      if ( -1 == rc )
      {
        fprintf( stderr, "error: %s\n", zmq_strerror( zmq_errno() ) );
      }
    }
  }

  return rc;
}

int YaSUBImpl::clearNotification( int key )
{
  // int rc = -1;
  char cKey[YaComponent::KeySize + 1];
  sprintf( cKey, YaComponent::KeyFmt, key );

  /*rc = */
  send( YaComponent::KeyClearNotification, YaComponent::KeySize, cKey );
  return -1;
}

void YaSUBImpl::setConnectionPara( const char* address, const char* ident )
{
  assert( 0 != mpReqRespSocket && 0 != address );

  assert( strlen( ident ) < YaComponent::MaxIdentSize );
  strncpy( mcIdent, ident, strlen( ident ) );

  if ( mpReqRespSocket && !mbConnected )
  {
    zmq_setsockopt( mpReqRespSocket, ZMQ_IDENTITY, mcIdent, strlen( mcIdent ) );
    int timeout = 0;
    zmq_setsockopt( mpReqRespSocket, ZMQ_RCVTIMEO, &timeout, sizeof( timeout ) );
    if ( 0 == zmq_connect( mpReqRespSocket, address ) )
    {
      mbConnected = true;
      char cKey[YaComponent::KeySize + 1];
      sprintf( cKey, YaComponent::KeyFmt, YaComponent::KeySync );
      auto rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
      assert( -1 != rc );
      rc = zmq_send( mpReqRespSocket, cKey, YaComponent::KeySize, 0 );
      assert( -1 != rc );
      std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
      bool bSync = false;
      do
      {
        QCoreApplication::processEvents();
        bSync = checkSync();
      } while ( !bSync );
    }
    else
    {
      fprintf( stderr, "error: cannot connect to address %s\n", address );
    }
  }
  else
  {
    if ( mbConnected )
    {
      fprintf( stderr, "error: already connected!\n" );
    }
  }
}

bool YaSUBImpl::checkSync()
{
  //  bool bRet = false;
  //  int rc = -1;
  //  int size = -1;
  //  if( 0 != mpReqRespSocket)
  //  {

  //    zmq_pollitem_t items [1];
  //    items[0].socket = mpReqRespSocket;
  //    items[0].events = ZMQ_POLLIN;

  //    rc = zmq_poll (items, 1, 0);

  //    if( 0 < items[0].revents )
  //    {
  ////      size = zmq_recv (mpReqRespSocket, 0, 0, ZMQ_NOBLOCK);
  ////      assert( 0 == size );
  //      size = zmq_recv (mpReqRespSocket, mMsgRespBuffer.data(),
  //      YaComponent::MaxMessageSize, ZMQ_NOBLOCK); if ( 0 < size )
  //      {
  //        bRet = ( 0 ==
  //        strncmp(YaComponent::SynAck,mMsgRespBuffer.data(),strlen(YaComponent::SynAck)))
  //        ? true : false;
  //      }
  //    }
  //  }

  return mbSync;
}

int YaSUBImpl::receive( int& key, int& size, char** pcData )
{
  int    iBytes   = 0;
  int    more     = -1;
  size_t moreSize = sizeof( more );

  if ( 0 != mpReqRespSocket )
  {
    zmq_pollitem_t items[1];
    items[0].socket  = mpReqRespSocket;
    items[0].events  = ZMQ_POLLIN;
    items[0].revents = -1;

    auto rc = zmq_poll( items, 1, 0 );

    if ( -1 < rc && 0 < items[0].revents )
    {
      int  rcvKey   = -1;
      auto keyBytes = zmq_recv( mpReqRespSocket, 0, 0, 0 );
      assert( 0 == keyBytes );
      char cKey[YaComponent::KeySize + 1];
      memset( cKey, 0, YaComponent::KeySize + 1 );
      keyBytes = zmq_recv( mpReqRespSocket, cKey, YaComponent::KeySize, 0 );
      assert( keyBytes == YaComponent::KeySize );
      if ( keyBytes != YaComponent::KeySize )
      {
        fprintf( stderr, "YaSUBImpl::receive:error: %s\n", zmq_strerror( zmq_errno() ) );
      }
      else
      {
        sscanf( cKey, YaComponent::KeyFmt, &rcvKey );
      }

      if ( 0 <= rcvKey )
      {
        key = rcvKey;
        zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
        if ( more )
        {
          iBytes = zmq_recv( mpReqRespSocket, mMsgRespBuffer.data(), YaComponent::MaxMessageSize, 0 );
          if ( 0 < iBytes )
          {
            size    = iBytes;
            *pcData = mMsgRespBuffer.data();
          }
          rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
          if ( more && -1 < rc )
          {
            qFatal( "YaSUBImpl::receive: KeyFmt unexpected end of message" );
          }
        }
        else
        {
          // qFatal("YaSUBImpl::receive: valid key, but not message content!");
        }
      }
      else if ( YaComponent::KeySync == rcvKey )
      {
        rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
        if ( -1 < rc && more )
        {
          auto keyBytes = zmq_recv( mpReqRespSocket, mMsgRespBuffer.data(), YaComponent::MaxMessageSize, 0 );
          if ( 0 < keyBytes )
          {
            mbSync = ( 0 == strncmp( YaComponent::SynAck, mMsgRespBuffer.data(), strlen( YaComponent::SynAck ) ) ) ? true : false;
            rc     = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
            if ( -1 < rc && more )
            {
              qFatal( "YaSUBImpl::receive: KeySync unexpected end of message" );
            }
          }
        }
      }
      else if ( YaComponent::KeyEnd == rcvKey )
      {
        qInfo( "YaSUBImpl::receive: KeyEnd, closing connection" );
        close();
      }
      else
      {
        qFatal( "%s", qPrintable( QString( "YaSUBImpl::receive %1" ).arg( rcvKey ) ) );
      }
      // miMessageCnt++;
    }
  }

  return iBytes;
}
