#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H

#include "YaComponent.h"
#include "YaBuffer.h"
#include <google/protobuf/message_lite.h>

#include <map>

class YaPUBImpl
{

public:
  YaPUBImpl( void* context );
  virtual ~YaPUBImpl();
//  bool bind( const char* address, const char* syncaddress) { setConnectionPara(address,syncaddress,5000); }
  void setConnectionPara(const char* pub, const char* req, int hwm = 0);
  int send(int key, const ::google::protobuf::MessageLite& msg );
  int response(int key, const ::google::protobuf::MessageLite& msg, const std::string& ident );
  int send(int key, int msgSize, const char* msgData );
  int send(int key, int msgSize, const char* msgData, const std::string& ident );
  int receive(int& key, int size, char** pcData, std::string& ident );
//  bool checkSubscribers(int iNumExpectedSubscribers = 1);
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
  char mcKey[YaComponent::KeySize + 1];
  char mcKeyReq[YaComponent::KeySize + 1];
  std::map<std::string, int> mPeerMap;
//  char mcSize[YaComponent::MessageSize];

};

#endif // YAPUBIMPL_H
