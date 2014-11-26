#include "publishercompunittest.h"
#include <QtCore/QtDebug>
#include <limits>

PublisherComp::PublisherComp(void* context)
  : YaComponent::PublisherCompImpl( context, static_cast<IPublisherIfcStubHandler&>(self()) )
  , IPublisherIfcStubHandler( self() )
  , miRequestStop( 0 )
  , miTimerID(-1)
  , miMessageCnt(0)
  , miMaxMessageCnt(1)
  , moData()
  , mbFinished(false)
{
}

void PublisherComp::init()
{
  PublisherCompImpl::init();
  moData.mutable_samples()->Reserve(4096);
}


void PublisherComp::onRequestStartData( int id )
{
  qDebug() << "received onRequestStartData, start sending";
  if( 0 < miTimerID)
  {
    qDebug() << "already started";
  }
  else
  {
    miMessageCnt = 0;
    timerEvent(0);
    miTimerID = QObject::startTimer(20);
    mbFinished = false;
  }
}

void PublisherComp::onRequestStopData( int id )
{
  qDebug() << "received onRequestStopData";
  miRequestStop++;
  QObject::killTimer(miTimerID);
  miTimerID = 0;
  qDebug() << "send %d messages" << miMessageCnt;
}

void PublisherComp::timerEvent( QTimerEvent* )
{
  int rc = -1;

  do
  {
    //moText.set_text(qPrintable(oText.arg(miMessageCnt)));
    moData.set_timeseconds(0);
    moData.set_timefraction(0.0);
    moData.set_counter(miMessageCnt);
    for(int idx = 0;idx < 2048; idx++)
    {
      //moData.set_samples(idx, rand() / std::numeric_limits<int>::max() );
    }

    rc = mReceiverData.send(YaComponent::PublisherIfcStub::PROP_DATA, moData );
    if( -1 != rc)
    {
      miMessageCnt++;
    }
    if( miMessageCnt >= miMaxMessageCnt )
    {
      QObject::killTimer(miTimerID);
      miTimerID = 0;
      mbFinished = true;
      break;
    }
  } while( rc != -1);
}

