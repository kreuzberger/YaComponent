#include "textgencompunittest.h"

TextGenComp::TextGenComp(void* context)
  : YaComponent::TextGenCompImpl( context, static_cast<ITextGenIfcStubHandler&>(self()) )
  , ITextGenIfcStubHandler( self() )
  , miRequestStop( 0 )
  , miTimerID(-1)
  , miMessageCnt(0)
  , miMaxMessageCnt(1)
  , moText()
  , mbSendAutoStop(false)
{
}

void TextGenComp::init()
{
  TextGenCompImpl::init();

}


void TextGenComp::onRequestStartText( int id, const TextGen::Request& oRequest, TextGen::startedText& oResponse )
{
  fprintf(stderr, "received onRequestStartText, start sending text\n");
  oResponse.mutable_id()->set_id(oRequest.id());
  miMessageCnt = 0;
  timerEvent(0);
  miTimerID = QObject::startTimer(20);
}

void TextGenComp::onRequestStopText( int id, const TextGen::Request& oRequest, TextGen::stoppedText& oResponse)
{
  fprintf(stderr, "received onRequestStopText\n");
  oResponse.mutable_id()->set_id(oRequest.id());
  miRequestStop++;
  QObject::killTimer(miTimerID);
  fprintf(stderr, "send %d messages\n", miMessageCnt);
}

void TextGenComp::sendResponseStop()
{
  TextGen::stoppedText oResponse;
  oResponse.mutable_id()->set_id(4242);
  mTextGenXml.response(YaComponent::TextGenIfcStub::RESP_TEXTGEN_STOPPEDTEXT,oResponse, "");
  QObject::killTimer(miTimerID);
  fprintf(stderr, "send %d messages\n", miMessageCnt);
}

void TextGenComp::timerEvent( QTimerEvent* )
{
  QString oText("%1: The quick brown fox jumps over the lazy dog 0123456789\n");
  int rc = -1;

  do
  {
    moText.set_text(qPrintable(oText.arg(miMessageCnt)));
    rc = mTextGenXml.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, moText);
   // rc = mTextGenPlain.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, moText);
    if( -1 != rc)
    {
      miMessageCnt++;
    }
    if( miMessageCnt >= miMaxMessageCnt )
    {
      QObject::killTimer(miTimerID);
      if( mbSendAutoStop ) sendResponseStop();
      break;
    }
  } while( rc != -1);
}

