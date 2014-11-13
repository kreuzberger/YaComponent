#ifndef YASUBIMPL_H
#define YASUBIMPL_H

#include "YaBuffer.h"
#include "YaComponent.h"
#include <google/protobuf/message_lite.h>

class YaSUBImpl
{
  public:
    YaSUBImpl( void* context );
    virtual ~YaSUBImpl();
    virtual void setConnectionPara(const char* sub, const char* req, const char* ident );
    virtual int receive(int& key, int& size, char** pcData );
    virtual int send( int key, int size, const char* );

    int request( int key, const ::google::protobuf::MessageLite& msg );
    int setNotification(int key);
    int clearNotification( int key);

    void close();
    int getMessageCnt() { return miMessageCnt; }
    bool checkSync();

  private:
    YaSUBImpl();
    YaSUBImpl( const YaSUBImpl& );
    YaSUBImpl& operator=( const YaSUBImpl& );

    void* mpSUBSocket;
    void* mpReqRespSocket;
    bool mbConnected;
    char mcKey[YaComponent::KeySize + 1];
    char mcKeyReq[YaComponent::KeySize + 1];
    char mcIdent[YaComponent::MaxIdentSize];
    YaBuffer mMsgOutBuffer;
    YaBuffer mMsgPropBuffer;
    YaBuffer mMsgRespBuffer;
    int miMessageCnt;
};
#endif // YASUBIMPL_H
