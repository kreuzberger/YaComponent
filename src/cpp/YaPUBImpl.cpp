#include "YaPUBImpl.h"
#include "zmq.h"
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <QtCore/QtDebug>

YaPUBImpl::YaPUBImpl(void * context)
  : mpReqRespSocket(0)
  , mbBound(false)
  , miSubscribersCnt(0)
  , mMsgBuffer(YaComponent::MaxMessageSize)
  , mMsgBufferReq(YaComponent::MaxMessageSize)
{
  assert( 0 != context );
  if( 0 != context )
  {
    mpReqRespSocket = zmq_socket (context, ZMQ_ROUTER);
  }
  else
  {
    fprintf(stderr, "error: invalid context\n");
  }
}

YaPUBImpl::~YaPUBImpl()
{
  zmq_close( mpReqRespSocket );
  mpReqRespSocket = 0;
}

void YaPUBImpl::close()
{
  send(YaComponent::KeyEnd,0,0,"");
  zmq_close( mpReqRespSocket );
  mpReqRespSocket = 0;
}


int YaPUBImpl::receive(int& key, int& size, char** pcData, std::string& ident )
{
  int iBytes = 0;
  char address[YaComponent::MaxIdentSize];
  int addressSize = 0;
  int rc = -1;
  int more = -1;
  size_t moreSize = sizeof(more);
  memset(mcKeyReq,0,YaComponent::KeySize+1);


  if( 0 != mpReqRespSocket )
  {
    zmq_pollitem_t items [1];
    items[0].socket = mpReqRespSocket;
    items[0].events = ZMQ_POLLIN;
    items[0].revents = -1;

    rc = zmq_poll (items, 1, 0);

    if( -1 < rc && 0 < items[0].revents )
    {
      int addressSize = zmq_recv( mpReqRespSocket, address, YaComponent::MaxIdentSize, ZMQ_NOBLOCK );
      if( 0 < addressSize )
      {
        ident = address;
        // identifier or address received
        assert( addressSize < YaComponent::MaxIdentSize);
        // receive empty block
        rc = zmq_recv( mpReqRespSocket, 0, 0, ZMQ_NOBLOCK );
        assert( 0 == rc );

        // receive key
        iBytes = zmq_recv (mpReqRespSocket, mcKeyReq, YaComponent::KeySize, ZMQ_NOBLOCK);
        if( 0 < iBytes )
        {
          assert (iBytes == YaComponent::KeySize);
          sscanf(mcKeyReq,YaComponent::KeyFmt,&key);
          {
            if( 0 <= key)
            {
              qDebug() << "peer "  << ident.c_str() << " with key " << key;

              rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
              if( -1 < rc && more )
              {
                iBytes = zmq_recv (mpReqRespSocket, mMsgBufferReq.data(), YaComponent::MaxMessageSize, ZMQ_NOBLOCK);
                if( 0 < iBytes )
                {
                  size = iBytes;
                  *pcData = mMsgBufferReq.data();
                }
                rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if( -1 < rc && more )
                {
                  assert(0);
                }
              }
            }
            else if( YaComponent::KeySync == key)
            {
              mPeerMap[ident][-1] = 0;
                // internal used sync key!
              zmq_send( mpReqRespSocket, address,addressSize, ZMQ_SNDMORE);
              zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE);
              zmq_send( mpReqRespSocket, mcKeyReq, YaComponent::KeySize, ZMQ_SNDMORE );
              zmq_send (mpReqRespSocket, YaComponent::SynAck, strlen (YaComponent::SynAck), 0);
              iBytes = 0;
              rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
              if( -1 < rc && more )
              {
                assert(0);
              }
            }
            else if( YaComponent::KeySetNotification == key)
            {
              qDebug() << "peer "  << ident.c_str() << " setNotification";
              char cNotKey[YaComponent::KeySize + 1];
              memset(cNotKey,0,YaComponent::KeySize+1);
              iBytes = zmq_recv (mpReqRespSocket,cNotKey, YaComponent::KeySize, ZMQ_NOBLOCK);
              assert( (0 < iBytes) && (iBytes == YaComponent::KeySize) );
              if( 0 < iBytes )
              {
                int notKey = -1;
                sscanf(cNotKey,YaComponent::KeyFmt,&notKey);
                qDebug() << "peer "  << ident.c_str() << " setNotification property " << notKey;
                mPeerMap[ident][notKey] = 1;
                rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if( -1 < rc && more )
                {
                  assert(0);
                }

              }
            }
            else if( YaComponent::KeyClearNotification == key)
            {
              char cNotKey[YaComponent::KeySize + 1];
              memset(cNotKey,0,YaComponent::KeySize+1);
              iBytes = zmq_recv (mpReqRespSocket, cNotKey, YaComponent::KeySize, ZMQ_NOBLOCK);
              assert( (0 < iBytes) && (iBytes == YaComponent::KeySize) );
              if( 0 < iBytes )
              {
                int notKey = -1;
                sscanf(cNotKey,YaComponent::KeyFmt,&notKey);
                mPeerMap[ident][notKey] = 0;
                rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if( -1 < rc && more )
                {
                  assert(0);
                }

              }
            }
            else if( YaComponent::KeyEnd == key)
            {
              qDebug() << "peer "  << ident.c_str() << " closes";
              mPeerMap.erase(ident);
              rc = zmq_getsockopt (mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
              if( -1 < rc && more )
              {
                assert(0);
              }

            }
            else
            {
              qDebug() << "unknown key " << key;
              assert(0);
            }
          }
        }
      }
    }
  }

  return iBytes;
}

void YaPUBImpl::setConnectionPara(const char* address, int hwm)
{
  assert( 0 != mpReqRespSocket && 0 != address );
  if( mpReqRespSocket && !mbBound )
  {
    if( 0 < hwm )
    {
      zmq_setsockopt(mpReqRespSocket,ZMQ_SNDHWM,&hwm,sizeof(hwm));
      int wait = 1;
      int timeout = 0;
      zmq_setsockopt (mpReqRespSocket, ZMQ_SNDTIMEO, &timeout, 4);
    }

    int mandatory = 1;
    zmq_setsockopt( mpReqRespSocket, ZMQ_ROUTER_MANDATORY, &mandatory,sizeof(mandatory));

    if( 0 == zmq_bind(mpReqRespSocket,address))
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
    if( !mpReqRespSocket )
    {
      fprintf(stderr, "error: no socket!\n");
    }
  }

 // return mbBound;

}

int YaPUBImpl::send(int key, const ::google::protobuf::MessageLite& msg)
{
  int rc = -1;
  int iSize = msg.ByteSize();
  msg.SerializeToArray(mMsgBuffer.data(),iSize);
  rc = send(key, iSize, mMsgBuffer.data());

  return rc;
}

int YaPUBImpl::response(int key, const ::google::protobuf::MessageLite& msg, const std::string& ident)
{
  int rc = -1;
  int iSize = msg.ByteSize();
  msg.SerializeToArray(mMsgBuffer.data(),iSize);
  rc = send(key, iSize, mMsgBuffer.data(), ident);

  return rc;
}


int YaPUBImpl::send(int key, int msgSize, const char* msgData)
{
  sprintf(mcKey,YaComponent::KeyFmt,key);
  int rc = -1;

  if( 0 != mpReqRespSocket )
  {
    for( std::map<std::string, std::map<int, int> >::const_iterator it = mPeerMap.begin() ; it != mPeerMap.end(); ++it )
    {
      if( 1 == mPeerMap[it->first][key])
      {
        rc = zmq_send( mpReqRespSocket, it->first.c_str(), it->first.length(), ZMQ_SNDMORE);
        if( -1 == rc )
        {
          break;
        }
        assert( it->first.length() == rc );
        rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE);
        assert( -1 != rc );

        int flags = (0 != msgSize && 0 != msgData) ? ZMQ_SNDMORE :  0;

        rc = zmq_send( mpReqRespSocket, mcKey, YaComponent::KeySize, flags );
        assert( YaComponent::KeySize == rc);

        if( 0 != msgSize && 0 != msgData )
        {
          rc = zmq_send(mpReqRespSocket, msgData, msgSize,0);
          assert( msgSize == rc );
        }
      }
    }
  }

  if( -1 == rc )
  {
	if( errno != EAGAIN )
	{
	  fprintf(stderr, "received error %s, exiting with error\n", zmq_strerror(errno));
	}
    assert( errno == EAGAIN );
  }

  return rc;
}

int YaPUBImpl::send(int key, int msgSize, const char* msgData, const std::string& ident)
{
  char cKey[YaComponent::KeySize + 1];
  int rc = -1;

  assert( 0 != mpReqRespSocket  );
  bool bResponseAll = ident.empty();

  if( 0 != mpReqRespSocket)
  {
    std::map<std::string, std::map<int, int> >::const_iterator itStart;
    std::map<std::string, std::map<int, int> >::const_iterator itEnd;

    if(!bResponseAll )
    {
      assert(1 == mPeerMap.count(ident));
      itStart = mPeerMap.find(ident);
      itEnd = itStart;
      ++itEnd;
    }
    else
    {
      itStart = mPeerMap.begin();
      itEnd = mPeerMap.end();
    }

    for( std::map<std::string, std::map<int, int> >::const_iterator it = itStart ; it != itEnd; ++it )
    {
      sprintf(cKey,YaComponent::KeyFmt,key);
        // internal used sync key!
      rc = zmq_send( mpReqRespSocket, it->first.c_str(), it->first.length(), ZMQ_SNDMORE);
      if( -1 == rc )
      {
        break;
      }
      assert( it->first.length() == rc );
      rc = zmq_send( mpReqRespSocket, 0, 0, ZMQ_SNDMORE);
      assert( -1 != rc );
      int flags = (0 != msgSize && 0 != msgData) ? ZMQ_SNDMORE :  0;
      rc = zmq_send( mpReqRespSocket, cKey, YaComponent::KeySize, flags );
      assert( YaComponent::KeySize == rc);
      if( 0 != msgSize && 0 != msgData )
      {
        rc = zmq_send(mpReqRespSocket, msgData, msgSize,0);
        assert( msgSize == rc );
      }
    }
  }
  return rc;
}

