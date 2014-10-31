#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H


#include <google/protobuf/message_lite.h>
#include "YaBuffer.h"

class YaPUBImpl
{

public:
  YaPUBImpl( void* context );
  virtual ~YaPUBImpl();
  bool bind( const char* address, const char* syncaddress);
  int send(int key, const ::google::protobuf::MessageLite* msg );
  int send(int key, int msgSize, const char* msgData );
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
};

#endif // YAPUBIMPL_H
