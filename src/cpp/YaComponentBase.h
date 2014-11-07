#ifndef YACOMPONENTBASE_H
#define YACOMPONENTBASE_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "IYaConnection.h"

class YaComponentBase : public QObject
{
  Q_OBJECT
public:
  explicit YaComponentBase(QObject *parent = 0);

signals:

public slots:

protected:
  QHash<QString,IYaConnection*> mConnectionHash;

};

#endif // YACOMPONENTBASE_H
