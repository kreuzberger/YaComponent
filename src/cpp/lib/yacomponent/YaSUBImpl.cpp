#include "YaSUBImpl.h"

#include "zmq.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>

#include <assert.h>
#include <memory.h>

YaSUBImpl::YaSUBImpl(void *context)
{
    assert(0 != context);
    if (0 != context) {
        YaComponent::sleep(1);
        mpReqRespSocket = zmq_socket(context, ZMQ_DEALER);
    } else {
        fprintf(stderr, "error: invalid context\n");
    }
    memset(mcIdent, 0, YaComponent::MaxIdentSize);
}

void YaSUBImpl::close()
{
    zmq_close(mpReqRespSocket);
}

YaSUBImpl::~YaSUBImpl()
{
    zmq_close(mpReqRespSocket);
}

int YaSUBImpl::setNotification(int key)
{
    int rc = -1;
    char cKey[YaComponent::KeySize + 1];
    sprintf(cKey, YaComponent::KeyFmt, key);

    rc = send(YaComponent::KeySetNotification, YaComponent::KeySize, cKey);
    return rc;
}

int YaSUBImpl::request(int key, const ::google::protobuf::MessageLite &msg)
{
    int rc = -1;
    int iSize = msg.ByteSizeLong();
    msg.SerializeToArray(mMsgOutBuffer.data(), iSize);
    assert(iSize == msg.GetCachedSize());
    rc = send(key, iSize, mMsgOutBuffer.data());
    return rc;
}

int YaSUBImpl::request(int key)
{
    int rc = -1;
    rc = send(key, 0, 0);
    return rc;
}

int YaSUBImpl::send(int key, int size, const char *data)
{
    sprintf(mcKey, YaComponent::KeyFmt, key);
    int rc = -1;
    assert(0 != mpReqRespSocket);
    int flags = (0 != size && 0 != data) ? ZMQ_SNDMORE : 0;
    if (0 != mpReqRespSocket) {
        rc = zmq_send(mpReqRespSocket, 0, 0, ZMQ_SNDMORE);
        assert(-1 != rc);
        rc = zmq_send(mpReqRespSocket, mcKey, YaComponent::KeySize, flags);
        assert(-1 != rc);
        if (0 != size && 0 != data) {
            rc = zmq_send(mpReqRespSocket, data, size, 0);
        }
    }

    return rc;
}

int YaSUBImpl::clearNotification(int key)
{
    int rc = -1;
    char cKey[YaComponent::KeySize + 1];
    sprintf(cKey, YaComponent::KeyFmt, key);

    rc = send(YaComponent::KeyClearNotification, YaComponent::KeySize, cKey);
    return -1;
}

void YaSUBImpl::setConnectionPara(const char *address, const char *ident)
{
    assert(0 != mpReqRespSocket && 0 != address);

    assert(strlen(ident) < YaComponent::MaxIdentSize);
    strcpy(mcIdent, ident);

    if (mpReqRespSocket && !mbConnected) {
        int rc = -1;
        zmq_setsockopt(mpReqRespSocket, ZMQ_IDENTITY, mcIdent, strlen(mcIdent));
        int timeout = 0;
        zmq_setsockopt(mpReqRespSocket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
        if (0 == zmq_connect(mpReqRespSocket, address)) {
            mbConnected = true;
            char cKey[YaComponent::KeySize + 1];
            sprintf(cKey, YaComponent::KeyFmt, YaComponent::KeySync);
            rc = zmq_send(mpReqRespSocket, 0, 0, ZMQ_SNDMORE);
            assert(-1 != rc);
            rc = zmq_send(mpReqRespSocket, cKey, YaComponent::KeySize, 0);
            assert(-1 != rc);
            YaComponent::sleep(20);
            bool bSync = false;
            do {
                QCoreApplication::processEvents();
                bSync = checkSync();
            } while (!bSync);
        } else {
            fprintf(stderr, "error: cannot connect to address %s\n", address);
        }
    } else {
        if (mbConnected) {
            fprintf(stderr, "error: already connected!\n");
        }
    }

    // return mbConnected;
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

int YaSUBImpl::receive(int &key, int &size, char **pcData)
{
    int iBytes = 0;
    int rc = -1;
    int more = -1;
    size_t moreSize = sizeof(more);
    memset(mcKey, 0, YaComponent::KeySize + 1);

    assert(0 != mpReqRespSocket);
    if (0 != mpReqRespSocket) {
        zmq_pollitem_t items[1];
        items[0].socket = mpReqRespSocket;
        items[0].events = ZMQ_POLLIN;
        items[0].revents = -1;

        rc = zmq_poll(items, 1, 0);

        if (-1 < rc && 0 < items[0].revents) {
            iBytes = zmq_recv(mpReqRespSocket, 0, 0, ZMQ_NOBLOCK);
            assert(0 == iBytes);
            iBytes = zmq_recv(mpReqRespSocket, mcKey, YaComponent::KeySize, ZMQ_NOBLOCK);
            assert(iBytes == YaComponent::KeySize);

            sscanf(mcKey, YaComponent::KeyFmt, &key);
            if (0 <= key) {
                rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if (more) {
                    iBytes = zmq_recv(mpReqRespSocket,
                                      mMsgRespBuffer.data(),
                                      YaComponent::MaxMessageSize,
                                      ZMQ_NOBLOCK);
                    if (0 < iBytes) {
                        size = iBytes;
                        *pcData = mMsgRespBuffer.data();
                    }
                    rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                    if (more) {
                        if (-1 < rc && more) {
                            assert(0);
                        }
                    }
                }
            } else if (YaComponent::KeySync == key) {
                iBytes = 0;
                rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if (-1 < rc && more) {
                    iBytes = zmq_recv(mpReqRespSocket,
                                      mMsgRespBuffer.data(),
                                      YaComponent::MaxMessageSize,
                                      ZMQ_NOBLOCK);
                    if (0 < iBytes) {
                        mbSync = (0
                                  == strncmp(YaComponent::SynAck,
                                             mMsgRespBuffer.data(),
                                             strlen(YaComponent::SynAck)))
                                     ? true
                                     : false;
                        rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                        if (-1 < rc && more) {
                            assert(0);
                        }
                    }
                }
            } else if (YaComponent::KeyEnd == key) {
                iBytes = 0;
                rc = zmq_getsockopt(mpReqRespSocket, ZMQ_RCVMORE, &more, &moreSize);
                if (-1 < rc && more) {
                    assert(0);
                }
            } else {
                qDebug() << "unknown key " << key;
                assert(0);
            }
            miMessageCnt++;
        }
    }

    return iBytes;
}
