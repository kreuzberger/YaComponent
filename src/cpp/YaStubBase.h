#ifndef YASTUBBASE_H
#define YASTUBBASE_H

#include <QtCore/QObject>

#include "YaPUBImpl.h"
class YaStubBase : public QObject
{
  Q_OBJECT
public:
  YaStubBase(void* context, QObject *parent = 0);

  void send(int key, int size, const char* msg );
//  int receive(int& key, int& size, const char* msg );
  void setConnectionPara( const char* pub, const char* req, int hwm  );

protected:
  YaPUBImpl mPublisher;

private:
  YaStubBase( const YaStubBase& );
  YaStubBase& operator=( const YaStubBase& );

};

#endif // YAPROXYBASE_H
