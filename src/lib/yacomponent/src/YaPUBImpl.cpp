#include "yacomponent/YaPUBImpl.h"
#include <assert.h>
#include <malloc.h>
#include <zmq.h>
#include <QtCore/QtDebug>

YaPUBImpl::YaPUBImpl( void* context )
{
  assert( 0 != context );
  if ( 0 != context )
  {
    mpReqRespSocket = zmq_socket( context, ZMQ_ROUTER );
  }
  else
  {
    fprintf( stderr, "error: invalid context\n" );
  }
}

YaPUBImpl::~YaPUBImpl()
{
  zmq_close( mpReqRespSocket );
  mpReqRespSocket = 0;
}

void YaPUBImpl::close()
{
  if ( mpReqRespSocket )
  {
    send( YaComponent::KeyEnd, 0, 0, "" );
    zmq_close( mpReqRespSocket );
    mpReqRespSocket = 0;
  }
}

int YaPUBImpl::receive( int& key, int& size, char** pcData, std::string& ident )
{
  int iBytes = 0;
  char address[YaComponent::MaxIdentSize + 1];
  memset( address, 0, YaComponent::MaxIdentSize + 1 );
  int more = -1;
  size_t moreSize = sizeof( more );

  if ( 0 != mpReqRespSocket )
  {
    zmq_pollitem_t items[1];
    items[0].socket = mpReqRespSocket;
    items[0].events = ZMQ_POLLIN;
    items[0].revents = -1;

    auto rc = zmq_poll( items, 1, 0 );

    if ( -1 < rc && 0 < items[0].revents )
    {
      auto addressSize = zmq_recv( mpReqRespSocket, address, YaComponent::MaxIdentSize, 0 );
      if ( 0 < addressSize )
      {
        ident = address;
        // identifier or address received
        assert( addressSize < YaComponent::MaxIdentSize );
        // receive empty block
        rc = zmq_recv( mpReqRespSocket, 0, 0, 0 );
        assert( 0 == rc );

        // receive key
        char cKey[YaComponent::KeySize + 1];
        memset( cKey, 0, YaComponent::KeySize + 1 );
        iBytes = zmq_recv( mpReqRespSocket, cKey, YaComponent::KeySize, 0 );
        if ( 0 < iBytes )
        {
          assert( iBytes == YaComponent::KeySize );
          auto ok = sscanf( cKey, YaComponent::KeyFmt, &key );
          if ( 0 < ok )
          {
            if ( 0 <= key )
            {
              qDebug() << "peer" << ident.c_str() << "with key" << key;

              rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
              if ( -1 < rc && more )
              {
                char sKey[YaComponent::MessageSize + 1];
                memset( sKey, 0, YaComponent::MessageSize + 1 );
                iBytes = zmq_recv( mpReqRespSocket, sKey, YaComponent::MessageSize, 0 );
                if ( 0 < iBytes )
                {
                  assert( iBytes == YaComponent::MessageSize );
                  int msgSize = 0;
                  ok = sscanf( sKey, YaComponent::MessageSizeFmt, &msgSize );
                  if ( ok )
                  {
                    if ( msgSize > mMsgBufferReq.size() )
                    {
                      qInfo() << "YaPUBImpl::receive msgsize" << msgSize << "exceeds current buffer size, resize";
                      mMsgBufferReq.resize( msgSize );
                    }
                    iBytes = zmq_recv( mpReqRespSocket, mMsgBufferReq.data(), msgSize, 0 );
                    if ( 0 < iBytes )
                    {
                      size = iBytes;
                      *pcData = mMsgBufferReq.data();
                    }
                    rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
                    if ( -1 < rc && more )
                    {
                      qFatal( "YaPUBImpl::receive unexpected end" );
                    }
                  }
                }
              }
            }
            else if ( YaComponent::KeySync == key )
            {
              mPeerMap[ident][-1] = 0;
              // internal used sync key!
              zmq_send( mpReqRespSocket, address, addressSize, ZMQ_SNDMORE );
              zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
              zmq_send( mpReqRespSocket, cKey, YaComponent::KeySize, ZMQ_SNDMORE );
              zmq_send( mpReqRespSocket, YaComponent::SynAck, strlen( YaComponent::SynAck ), 0 );
              iBytes = 0;
              rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
              if ( -1 < rc && more )
              {
                qFatal( "YaPUBImpl::receive KeySync unexpected end" );
              }
            }
            else if ( YaComponent::KeySetNotification == key )
            {
              qDebug() << "peer" << ident.c_str() << "setNotification";
              char sKey[YaComponent::MessageSize + 1];
              memset( sKey, 0, YaComponent::MessageSize + 1 );
              iBytes = zmq_recv( mpReqRespSocket, sKey, YaComponent::MessageSize, 0 );
              if ( 0 < iBytes )
              {
                assert( iBytes == YaComponent::MessageSize );
                int msgSize = 0;
                ok = sscanf( sKey, YaComponent::MessageSizeFmt, &msgSize );
                assert( msgSize == YaComponent::KeySize );
                if ( ok && msgSize == YaComponent::KeySize )
                {
                  char cNotKey[YaComponent::KeySize + 1];
                  memset( cNotKey, 0, YaComponent::KeySize + 1 );
                  iBytes = zmq_recv( mpReqRespSocket, cNotKey, YaComponent::KeySize, 0 );
                  assert( ( 0 < iBytes ) && ( iBytes == YaComponent::KeySize ) );
                  if ( 0 < iBytes )
                  {
                    int notKey = -1;
                    sscanf( cNotKey, YaComponent::KeyFmt, &notKey );
                    qDebug() << "peer" << ident.c_str() << "setNotification property " << notKey;
                    mPeerMap[ident][notKey] = 1;
                    rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
                    if ( -1 < rc && more )
                    {
                      qFatal( "YaPUBImpl::receive KeySetNotification unexpected end" );
                    }
                    // send values from LVC Cache
                    auto lvc = LVC();
                    {
                      const std::lock_guard<std::mutex> lock( mLVCMutex );
                      if ( mLVC.find( notKey ) != mLVC.end() )
                      {
                        lvc = mLVC[notKey];
                      }
                    }
                    if ( 0 < lvc.msgSize && !lvc.msg.empty() )
                    {
                      qDebug() << "YaPUBImpl::receive: sending LVC cache values on "
                                  "setNotification: size"
                               << lvc.msgSize << "msg" << lvc.msg.c_str();
                      send( notKey, lvc.msgSize, lvc.msg.c_str() );
                    }
                    else if ( 0 == lvc.msgSize /*&& lvc.msg.empty() */ )
                    {
                      qDebug() << "YaPUBImpl::receive: sending default LVC cache values on "
                                  "setNotification: size"
                               << lvc.msgSize << "msg" << lvc.msg.c_str();
                      send( notKey, lvc.msgSize, lvc.msg.c_str() );
                    }
                  }
                }
              }
            }
            else if ( YaComponent::KeyClearNotification == key )
            {
              char sKey[YaComponent::MessageSize + 1];
              memset( sKey, 0, YaComponent::MessageSize + 1 );
              iBytes = zmq_recv( mpReqRespSocket, sKey, YaComponent::MessageSize, 0 );
              if ( 0 < iBytes )
              {
                assert( iBytes == YaComponent::MessageSize );
                int msgSize = 0;
                ok = sscanf( sKey, YaComponent::MessageSizeFmt, &msgSize );
                assert( msgSize == YaComponent::KeySize );
                if ( ok && msgSize == YaComponent::KeySize )
                {
                  char cNotKey[YaComponent::KeySize + 1];
                  memset( cNotKey, 0, YaComponent::KeySize + 1 );
                  iBytes = zmq_recv( mpReqRespSocket, cNotKey, YaComponent::KeySize, 0 );
                  assert( ( 0 < iBytes ) && ( iBytes == YaComponent::KeySize ) );
                  if ( 0 < iBytes )
                  {
                    int notKey = -1;
                    sscanf( cNotKey, YaComponent::KeyFmt, &notKey );
                    mPeerMap[ident][notKey] = 0;
                    rc = zmq_getsockopt( mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize );
                    if ( -1 < rc && more )
                    {
                      qFatal( "YaPUBImpl::receive KeyClearNotification unexpected end" );
                    }
                  }
                }
              }
            }
            else if ( YaComponent::KeyEnd == key )
            {
              qDebug() << "peer " << ident.c_str() << " closes";
              mPeerMap.erase( ident );
              // rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
              // if (-1 < rc && more) {
              //     qFatal("YaPUBImpl::receive KeyEnd unexpected end");
              // }
              close();
            }
            else
            {
              qFatal( "%s", qPrintable( QString( "unknown key %1 from iBytes %2 to '%3'" ).arg( key ).arg( iBytes ).arg( cKey ) ) );
            }
          }
          else
          {
            qFatal( "%s", qPrintable( QString( "YaPUBImpl::receive: seemed to receive partial "
                                               "message key %1 from iBytes %2 to '%3' from ident %4" )
                                        .arg( key )
                                        .arg( iBytes )
                                        .arg( cKey, address ) ) );

            iBytes = 0;
            key = -1;
          }
        }
      }
    }
  }

  return iBytes;
}

void YaPUBImpl::setConnectionPara( const char* address, int hwm )
{
  assert( 0 != mpReqRespSocket && 0 != address );
  if ( mpReqRespSocket && !mbBound )
  {
    if ( 0 < hwm )
    {
      zmq_setsockopt( mpReqRespSocket, ZMQ_SNDHWM, &hwm, sizeof( hwm ) );
      //      int wait = 1;
      int timeout = 0;
      zmq_setsockopt( mpReqRespSocket, ZMQ_SNDTIMEO, &timeout, 4 );
    }

    int mandatory = 1;
    zmq_setsockopt( mpReqRespSocket, ZMQ_ROUTER_MANDATORY, &mandatory, sizeof( mandatory ) );

    if ( 0 == zmq_bind( mpReqRespSocket, address ) )
    {
      mbBound = true;
    }
    else
    {
      fprintf( stderr, "error: cannot bind to address %s\n", address );
    }
  }
  else
  {
    if ( mbBound )
    {
      fprintf( stderr, "error: already bound!\n" );
    }
    if ( !mpReqRespSocket )
    {
      fprintf( stderr, "error: no socket!\n" );
    }
  }

  // return mbBound;
}

int YaPUBImpl::send( int key, const ::google::protobuf::MessageLite& msg )
{
  int rc = -1;
  int iSize = msg.ByteSizeLong();
  msg.SerializeToArray( mMsgBuffer.data(), iSize );
  rc = send( key, iSize, mMsgBuffer.data() );

  return rc;
}

int YaPUBImpl::response( int key, const std::string& ident, const ::google::protobuf::MessageLite& msg )
{
  int rc = -1;
  int iSize = msg.ByteSizeLong();
  msg.SerializeToArray( mMsgBuffer.data(), iSize );
  rc = send( key, iSize, mMsgBuffer.data(), ident );

  return rc;
}

int YaPUBImpl::response( int key, const std::string& ident )
{
  int rc = -1;
  rc = send( key, 0, nullptr, ident );

  return rc;
}

int YaPUBImpl::send( int key, int msgSize, const char* msgData )
{
  int rc = -1;

  if ( 0 != mpReqRespSocket )
  {
    char cKey[YaComponent::KeySize + 1];
    memset( cKey, 0, YaComponent::KeySize + 1 );
    sprintf( cKey, YaComponent::KeyFmt, key );

    {
      const std::lock_guard<std::mutex> lock( mLVCMutex );
      mLVC[key] = { msgSize, msgData };
    }

    for ( auto it = mPeerMap.begin(); it != mPeerMap.end(); ++it )
    {
      // 1 means notification is set / enabled.
      if ( 1 == mPeerMap[it->first][key] )
      {
        rc = zmq_send( mpReqRespSocket, it->first.c_str(), it->first.length(), ZMQ_SNDMORE );
        if ( -1 == rc )
        {
          // something went wrong
          // fprintf(stderr,
          //         "YaPUBImpl::send::error %s from sending to peer %s\n",
          //         zmq_strerror(errno),
          //         it->first.c_str());
          break;
        }
        assert( it->first.length() == rc );
        rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
        assert( -1 != rc );

        auto send_more = ( 0 != msgSize && nullptr != msgData );

        int flags = send_more ? ZMQ_SNDMORE : 0;

        rc = zmq_send( mpReqRespSocket, cKey, YaComponent::KeySize, flags );
        assert( YaComponent::KeySize == rc );

        if ( send_more )
        {
          char sKey[YaComponent::MessageSize + 1];
          sprintf( sKey, YaComponent::MessageSizeFmt, msgSize );
          rc = zmq_send( mpReqRespSocket, sKey, YaComponent::MessageSize, flags );
          assert( -1 != rc );

          rc = zmq_send( mpReqRespSocket, msgData, msgSize, 0 );
          assert( msgSize == rc );
        }

        if ( -1 == rc )
        {
          if ( errno != EAGAIN )
          {
            fprintf( stderr, "YaPUBImpl::send::received error %s from sending to peer %s\n", zmq_strerror( errno ), it->first.c_str() );
          }
          assert( errno == EAGAIN );
        }
      }
    }
  }

  return rc;
}

int YaPUBImpl::send( int key, int msgSize, const char* msgData, const std::string& ident )
{
  int rc = -1;

  assert( 0 != mpReqRespSocket );
  bool bResponseAll = ident.empty();

  if ( 0 != mpReqRespSocket )
  {
    std::map<std::string, std::map<int, int>>::const_iterator itStart;
    std::map<std::string, std::map<int, int>>::const_iterator itEnd;

    if ( !bResponseAll )
    {
      assert( 1 == mPeerMap.count( ident ) );
      itStart = mPeerMap.find( ident );
      itEnd = itStart;
      ++itEnd;
    }
    else
    {
      itStart = mPeerMap.begin();
      itEnd = mPeerMap.end();
    }

    for ( std::map<std::string, std::map<int, int>>::const_iterator it = itStart; it != itEnd; ++it )
    {
      char cKey[YaComponent::KeySize + 1];
      sprintf( cKey, YaComponent::KeyFmt, key );
      // internal used sync key!
      rc = zmq_send( mpReqRespSocket, it->first.c_str(), it->first.length(), ZMQ_SNDMORE );
      if ( -1 == rc )
      {
        break;
      }
      assert( it->first.length() == rc );
      rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE );
      assert( -1 != rc );
      auto send_more = ( 0 != msgSize && nullptr != msgData );
      int flags = send_more ? ZMQ_SNDMORE : 0;
      rc = zmq_send( mpReqRespSocket, cKey, YaComponent::KeySize, flags );
      assert( YaComponent::KeySize == rc );
      if ( send_more )
      {
        char sKey[YaComponent::MessageSize + 1];
        sprintf( sKey, YaComponent::MessageSizeFmt, msgSize );
        rc = zmq_send( mpReqRespSocket, sKey, YaComponent::MessageSize, flags );
        assert( -1 != rc );

        rc = zmq_send( mpReqRespSocket, msgData, msgSize, 0 );
        assert( msgSize == rc );
      }
    }
  }
  return rc;
}
