#include "PubSubUnittestSP_PUB.h"
#include <QtTest>

#include <QtCore/QtDebug>

PublisherComp::PublisherComp(void *context)
    : YaComponent::PublisherCompImpl(context, static_cast<IPublisherIfcStubHandler &>(*this))
    , IPublisherIfcStubHandler(*this)
    , moData()
{}

void PublisherComp::init()
{
    PublisherCompImpl::init();
    moData.mutable_samples()->Reserve(4096);
}

void PublisherComp::onRequestStartData(int id)
{
    qDebug() << "received onRequestStartData, start sending";
}

void PublisherComp::onRequestStopData(int id)
{
    qDebug() << "received onRequestStopData";
}

int PublisherComp::sendData(const Data &data)
{
    moData = data;
    return mReceiverData.send(YaComponent::PublisherIfcStub::PROP_DATA, data);
}
