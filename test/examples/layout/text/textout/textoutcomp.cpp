#include "textoutcomp.h"

TextOutComp::TextOutComp( void* context )
 : ITextGenIfcProxyHandler( self() )
, YaComponent::TextOutCompImpl( context, static_cast<ITextGenIfcProxyHandler&>(self()) )
{
//  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);

}

void TextOutComp::init()
{
  mXml.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
  mPlain.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);

  TextGen::Request oRequest;
  oRequest.set_id(1);
  mXml.requestStartText(oRequest);
}

void TextOutComp::onProperty( const YaProxyBase* sender, const TextGen::Text& text)
{
  fprintf(stderr, "recevied property text %s\n", text.DebugString().c_str());
}
