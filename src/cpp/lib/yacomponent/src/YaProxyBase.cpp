#include <yacomponent/YaProxyBase.h>

YaProxyBase::YaProxyBase(void *context, int id, QObject *parent)
    : QObject(parent)
    , mSubscriber(context)
    , mId(id)
{}

int YaProxyBase::setNotification(int key)
{
    int rc = -1;
    rc = mSubscriber.setNotification(key);
    return rc;
}

int YaProxyBase::clearNotification(int key)
{
    int rc = -1;
    rc = mSubscriber.clearNotification(key);
    return rc;
}

void YaProxyBase::setConnectionPara(const char *address, const char *ident)
{
    mSubscriber.setConnectionPara(address, ident);
}

void YaProxyBase::close()
{
    mSubscriber.close();
}
