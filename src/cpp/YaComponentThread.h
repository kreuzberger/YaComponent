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
  virtual void run() {printf("starting thread\n");exec();printf("exiting thread\n");}
};

#endif // YACOMPONENTTHREAD_H
