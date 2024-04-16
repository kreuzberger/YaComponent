#pragma once
#include <QObject>

#include <yacomponent/YaComponent.h>
#include <yacomponent/YaComponentThread.h>

#include "IPublisherIfcStubHandler.h"
#include "PublisherCompImpl.h"

class PublisherComp : public YaComponent::PublisherCompImpl, public YaComponent::IPublisherIfcStubHandler
{
public:
    explicit PublisherComp(void *context);
    ~PublisherComp() override {}

    void onRequestStartData(int id) override;
    void onRequestStopData(int id) override;
    void init() override;
    void sendResponseStop();
    int sendData(const Data &data);
public:
    Data moData;
};
