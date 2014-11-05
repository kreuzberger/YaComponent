#ifndef YAPROXYBASE_H
#define YAPROXYBASE_H

#include <QtCore/QObject>

class YaProxyBase : public QObject
{
  Q_OBJECT
public:
  explicit YaProxyBase(QObject *parent = 0);

signals:

public slots:

private:
  YaProxyBase( const YaProxyBase& );
  YaProxyBase& operator=( const YaProxyBase& );

};

#endif // YAPROXYBASE_H
