#ifndef YACOMPONENTTHREAD_H
#define YACOMPONENTTHREAD_H

#include <QtCore/QThread>
#include <stdio.h>

class YaComponentThread: public QThread
{
  Q_OBJECT
public:
  YaComponentThread();
  virtual ~YaComponentThread();
protected:
  virtual void run() { exec(); }
};

#endif // YACOMPONENTTHREAD_H
