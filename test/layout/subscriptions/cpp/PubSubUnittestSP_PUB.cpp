#include "PubSubUnittestSP_PUB.h"
#include <QtTest>

#include <QtCore/QtDebug>

PublisherComp::PublisherComp( void* context )
  : YaComponent::PublisherCompImpl( context, static_cast<IPublisherIfcStubHandler&>( *this ) )
  , IPublisherIfcStubHandler( *this )
  , mData()
  , mTime()
{
}

void PublisherComp::init()
{
  PublisherCompImpl::init();
  mData.mutable_samples()->Reserve( 4096 );
}

void PublisherComp::onRequestStartData( int id )
{
  qDebug() << "received onRequestStartData, start sending";
}

void PublisherComp::onRequestStopData( int id )
{
  qDebug() << "received onRequestStopData";
}

int PublisherComp::sendData( const Data& data )
{
  mData = data;
  return mReceiverData.send( YaComponent::PublisherIfcStub::PROP_DATA, data );
}

int PublisherComp::sendTime( const Time& time )
{
  mTime = time;
  return mReceiverData.send( YaComponent::PublisherIfcStub::PROP_TIME, time );
}
