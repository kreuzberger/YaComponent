#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H

#include "YaComponent.h"
#include "YaBuffer.h"
#include <google/protobuf/message_lite.h>

class YaPUBImpl
{

public:
  YaPUBImpl( void* context );
  virtual ~YaPUBImpl();
//  bool bind( const char* address, const char* syncaddress) { setConnectionPara(address,syncaddress,5000); }
  void setConnectionPara(const char* pub, const char* req, int hwm = 0);
  int send(int key, const ::google::protobuf::MessageLite& msg );
  int send(int key, int msgSize, const char* msgData );
  int receive(int& key, int size, const  char* pcData );
  bool checkSubscribers(int iNumExpectedSubscribers = 1);
  void close();

private:
  YaPUBImpl();
  YaPUBImpl( const YaPUBImpl& );
  YaPUBImpl& operator=( const YaPUBImpl& );
  void resizeBuffer(int iLength);

  void* mpPUBSocket;
  void* mpReqRespSocket;
  bool mbBound;
  int miSubscribersCnt;
  YaBuffer mMsgBuffer;
  YaBuffer mMsgBufferReq;
  char mcKey[YaComponent::KeySize];
  char mcKeyReq[YaComponent::KeySize];
//  char mcSize[YaComponent::MessageSize];

};

#endif // YAPUBIMPL_H
