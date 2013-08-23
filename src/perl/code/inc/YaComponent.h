#ifndef IYACOMPONENT_H
#define IYACOMPONENT_H

#include <QtCore/QObject>

class YaComponent: public QObject
{
  Q_OBJECT
public:
  YaComponent():QObject() {}
  virtual ~YaComponent() {}
  
};

#endif 
