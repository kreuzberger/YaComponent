#ifndef TEXTOUTCOMPUNITTEST_H
#define TEXTOUTCOMPUNITTEST_H

#include "ITextGenIfcProxyHandler.h"
#include "TextOutCompImpl.h"

class TextOutComp : public YaComponent::TextOutCompImpl, public YaComponent::ITextGenIfcProxyHandler
{
public:
    TextOutComp(void *context);
    virtual ~TextOutComp() {}

    void onProperty(int, const TextGen::Text &);

    virtual void onResponse(int, const TextGen::startedText &);
    virtual void onResponse(int, const TextGen::stoppedText &);

    virtual void init();

    void setNotifications();
    void clearNotifications();

    void requestStart();
    void requestStop();

    int miPropertiesCnt;
    int miResponseStartCnt;
    int miResponseStopCnt;

private:
    TextOutComp &self() { return *this; }
};

#endif // TEXTOUTCOMP_H
