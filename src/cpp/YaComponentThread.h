#pragma once
#include <stdio.h>
#include <QtCore/QThread>

class YaComponentThread : public QThread
{
    Q_OBJECT
public:
    YaComponentThread();
    virtual ~YaComponentThread();

protected:
    virtual void run() { exec(); }
};
