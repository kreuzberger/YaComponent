#ifndef YASTUBBASE_H
#define YASTUBBASE_H

#include <QtCore/QObject>

class YaStubBase : public QObject
{
  Q_OBJECT
public:
  explicit YaStubBase(QObject *parent = 0);

signals:

public slots:

private:
  YaStubBase( const YaStubBase& );
  YaStubBase& operator=( const YaStubBase& );

};

#endif // YAPROXYBASE_H
