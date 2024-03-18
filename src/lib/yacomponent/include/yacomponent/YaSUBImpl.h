#pragma once

#include "YaBuffer.h"
#include "YaComponent.h"
#include <google/protobuf/message_lite.h>

class YaSUBImpl
{
public:
    YaSUBImpl(void *context);
    virtual ~YaSUBImpl();
    virtual void setConnectionPara(const char *address, const char *ident);
    virtual int receive(int &key, int &size, char **pcData);
    virtual int send(int key, int size, const char *);

    int request(int key, const ::google::protobuf::MessageLite &msg);
    int request(int key);
    int setNotification(int key);
    int clearNotification(int key);

    void close();
    int getMessageCnt() { return miMessageCnt; }
    bool checkSync();

private:
    YaSUBImpl();
    YaSUBImpl(const YaSUBImpl &);
    YaSUBImpl &operator=(const YaSUBImpl &);

    void *mpReqRespSocket = nullptr;
    bool mbConnected = false;
    char mcKey[YaComponent::KeySize + 1];
    char mcKeyReq[YaComponent::KeySize + 1];
    char mcIdent[YaComponent::MaxIdentSize];
    YaBuffer mMsgOutBuffer = YaBuffer(YaComponent::MessageSize);
    YaBuffer mMsgRespBuffer = YaBuffer(YaComponent::MaxMessageSize);
    int miMessageCnt = 0;
    bool mbSync = false;
};
