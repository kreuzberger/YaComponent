#ifndef PUBLISHERCOMP_H
#define PUBLISHERCOMP_H

#include "IPublisherIfcStubHandler.h"
#include "PublisherCompImpl.h"

class PublisherComp : public YaComponent::PublisherCompImpl, public YaComponent::IPublisherIfcStubHandler
{
public:
    PublisherComp(void *context);
    virtual ~PublisherComp() {}

    virtual void onRequestStartData(int id);
    virtual void onRequestStopData(int id);
    virtual void init();
    int miRequestStop;
    int miTimerID;
    int miMessageCnt;
    int miMaxMessageCnt;
    bool mbFinished;

    void sendResponseStop();

protected:
    virtual void timerEvent(QTimerEvent *);

private:
    PublisherComp &self() { return *this; }
    Data moData;
};

#endif // TEXTGENCOMP_H
