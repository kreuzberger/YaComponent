#pragma once
#include "ITextGenIfcStubHandler.h"
#include "TextGenCompImpl.h"

class TextGenComp : public YaComponent::TextGenCompImpl, public YaComponent::ITextGenIfcStubHandler
{
public:
    TextGenComp(void *context);
    virtual ~TextGenComp() {}

    void onRequestStartText( int id, const TextGen::Request&, TextGen::RequestInfo& ) override;
    void onRequestStopText( int id, const TextGen::Request&, TextGen::RequestInfo& ) override;
    void onRequestTerminate(int id) override;
    void init() override;
    int miRequestStop;
    int miTimerID;
    int miMessageCnt;
    int miMaxMessageCnt;
    bool mbSendAutoStop;

    void sendResponseStop();

protected:
    void timerEvent(QTimerEvent *) override;

private:
    TextGen::Text moText;
};
