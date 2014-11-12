#include "textoutcompunittest.h"

TextOutComp::TextOutComp( void* context )
 : ITextGenIfcProxyHandler( self() )
, YaComponent::TextOutCompImpl( context, static_cast<ITextGenIfcProxyHandler&>(self()) )
, miPropertiesCnt(0)
, miResponseStartCnt(0)
, miResponseStopCnt(0)
{
//  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);

}

void TextOutComp::init()
{
  TextOutCompImpl::init();

}

void TextOutComp::setNotifications()
{
  mXml.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
  mPlain.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
}

void TextOutComp::requestStart()
{
  TextGen::Request oRequest;
  oRequest.set_id(1);
  mXml.requestStartText(oRequest);
  mXml.receive();

}

void TextOutComp::requestStop()
{
  TextGen::Request oRequest;
  oRequest.set_id(1);
  mXml.requestStopText(oRequest);
  mXml.receive();

}


void TextOutComp::onProperty( int, const TextGen::Text& text)
{
  fprintf(stderr, "recevied property text %s\n", text.DebugString().c_str());
  miPropertiesCnt++;
}
