#pragma once
#include <QtCore/QObject>
#include <QtCore/QtDebug>

#include "YaPUBImpl.h"
class YaStubBase : public QObject
{
    Q_OBJECT
public:
    YaStubBase(void *context, int id, QObject *parent = 0);

    void send(int key, int size, const char *msg);
    void setConnectionPara(const char *address, int hwm);
    void close();

protected:
    YaPUBImpl mPublisher;
    int mId;

private:
    YaStubBase(const YaStubBase &);
    YaStubBase &operator=(const YaStubBase &);
};
