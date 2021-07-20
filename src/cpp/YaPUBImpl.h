#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H

#include "YaBuffer.h"
#include "YaComponent.h"
#include <google/protobuf/message_lite.h>

#include <map>

class YaPUBImpl {

public:
  YaPUBImpl(void *context);
  virtual ~YaPUBImpl();
  //  bool bind( const char* address, const char* syncaddress) {
  //  setConnectionPara(address,syncaddress,5000); }
  void setConnectionPara(const char *address, int hwm = 0);
  int send(int key, const ::google::protobuf::MessageLite &msg);
  int response(int key, const ::google::protobuf::MessageLite &msg,
               const std::string &ident);
  int send(int key, int msgSize, const char *msgData);
  int send(int key, int msgSize, const char *msgData, const std::string &ident);
  int receive(int &key, int &size, char **pcData, std::string &ident);
  //  bool checkSubscribers(int iNumExpectedSubscribers = 1);
  void close();

private:
  YaPUBImpl();
  YaPUBImpl(const YaPUBImpl &);
  YaPUBImpl &operator=(const YaPUBImpl &);
  void resizeBuffer(int iLength);

  void *mpReqRespSocket = nullptr;
  bool mbBound = false;
  int miSubscribersCnt = 0;
  YaBuffer mMsgBuffer = {};
  YaBuffer mMsgBufferReq = {};
  char mcKey[YaComponent::KeySize + 1] = {};
  char mcKeyReq[YaComponent::KeySize + 1] = {};
  std::map<std::string, std::map<int, int>> mPeerMap = {};
  //  char mcSize[YaComponent::MessageSize];
};

#endif // YAPUBIMPL_H
