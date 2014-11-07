#ifndef IYACONNECTION_H
#define IYACONNECTION_H

#include <google/protobuf/message_lite.h>

class IYaConnection
{
public:
  IYaConnection() {}
  virtual ~IYaConnection() {}

  virtual void setConnectionPara(const char* pub, const char* req, int hwm = 0) = 0;
  virtual int send(int key, const ::google::protobuf::MessageLite& msg ) = 0;
  virtual int send(int key, int msgSize, const char* msgData ) = 0;
  virtual int receive(int& key, int size, const  char* pcData ) = 0;

};

#endif // IYACONNECTION_H
