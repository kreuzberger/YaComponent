#include "textgencompunittest.h"

TextGenComp::TextGenComp(void* context)
  : YaComponent::TextGenCompImpl( context, static_cast<ITextGenIfcStubHandler&>(self()) )
  , ITextGenIfcStubHandler( self() )
{
}

void TextGenComp::init()
{
  TextGenCompImpl::init();

}


void TextGenComp::onRequestStartText( int id, const TextGen::Request& oRequest, TextGen::startedText& oResponse )
{
  fprintf(stderr, "received onRequestStartText, start sending text\n");

  TextGen::Text oText;
  oText.set_text("This is my text text");
  mTextGenPlain.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, oText);
  oText.set_text("<xml>This is my text text</xml>");
  mTextGenXml.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, oText);

  oResponse.mutable_id()->set_id(oRequest.id());
}

void TextGenComp::onRequestStopText( int id, const TextGen::Request& oRequest, TextGen::stoppedText& oResponse)
{
  oResponse.mutable_id()->set_id(oRequest.id());

}

