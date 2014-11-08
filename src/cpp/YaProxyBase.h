#ifndef YAPROXYBASE_H
#define YAPROXYBASE_H

#include "YaSUBImpl.h"

#include <QtCore/QObject>



class YaProxyBase : public QObject
{
  Q_OBJECT
public:
  YaProxyBase(void* context, int id, QObject *parent = 0);


  void setNotification( int  );
  void clearNotification( int );

  void setConnectionPara( const char*, const char*, int hwm = 0 );

  virtual int receive() = 0;

signals:

public slots:

protected:
  YaSUBImpl mSubscriber;
  int mId;

private:
  YaProxyBase( const YaProxyBase& );
  YaProxyBase& operator=( const YaProxyBase& );

};

#endif // YAPROXYBASE_H
