#pragma once
#include "ITextGenIfcStubHandler.h"
#include "TextGenCompImpl.h"

class TextGenComp : public YaComponent::TextGenCompImpl, public YaComponent::ITextGenIfcStubHandler
{
public:
    TextGenComp(void *context);
    virtual ~TextGenComp() {}

    virtual void onRequestStartText(int id, const TextGen::Request &, TextGen::startedText &);
    virtual void onRequestStopText(int id, const TextGen::Request &, TextGen::stoppedText &);
    virtual void init();
    int miRequestStop;
    int miTimerID;
    int miMessageCnt;
    int miMaxMessageCnt;
    bool mbSendAutoStop;

    void sendResponseStop();

protected:
    virtual void timerEvent(QTimerEvent *);

private:
    TextGenComp &self() { return *this; }
    TextGen::Text moText;
};
