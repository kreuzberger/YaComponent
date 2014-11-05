#include "textgencomp.h"

TextGenComp::TextGenComp(void* context)
  : YaComponent::TextGenCompImpl( context, static_cast<ITextGenIfcStubHandler&>(self()) )
  , ITextGenIfcStubHandler( self() )
{
}

void TextGenComp::onRequestStartText(  const TextGen::Request& )
{
  TextGen::Text oText;
  oText.set_text("This is my text text");
  mTextGenPlain.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, oText);
  oText.set_text("<xml>This is my text text</xml>");
  mTextGenXml.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, oText);
}

void TextGenComp::onRequestStopText(  const TextGen::Request& )
{

}

