#ifndef TEXTOUTCOMPUNITTEST_H
#define TEXTOUTCOMPUNITTEST_H

#include "ITextGenIfcProxyHandler.h"
#include "TextOutCompImpl.h"

class TextOutComp : public YaComponent::TextOutCompImpl, public YaComponent::ITextGenIfcProxyHandler
{
public:
    TextOutComp(void *context);
    virtual ~TextOutComp() {}

    void onPropertyText(int, const TextGen::Text &) override;

    void onResponseStartedText(int, const TextGen::StartedText &) override;
    void onResponseStoppedText(int, const TextGen::StoppedText &) override;
    void onResponseTerminated(int) override;

    void init() override;

    void setNotifications();
    void clearNotifications();

    void requestStart();
    void requestStop();
    void requestTerminate();

    int miPropertiesCnt;
    int miResponseStartCnt;
    int miResponseStopCnt;
    int miResponseTerminatedCnt;

private:
};

#endif // TEXTOUTCOMP_H
