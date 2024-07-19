#pragma once
#include "YaSUBImpl.h"

#include <QtCore/QObject>
#include <QtCore/QtDebug>

class YaProxyBase : public QObject
{
  Q_OBJECT
public:
  YaProxyBase( void* context, int id, QObject* parent = 0 );

  int setNotification( int );
  int clearNotification( int );

  void setConnectionPara( const char*, const char* ident );
  void close();

  virtual void receive() = 0;

signals:

public slots:

protected:
  YaSUBImpl mSubscriber;
  int mId;

private:
  YaProxyBase( const YaProxyBase& );
  YaProxyBase& operator=( const YaProxyBase& );
};
