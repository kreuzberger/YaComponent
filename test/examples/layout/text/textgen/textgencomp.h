#ifndef TEXTGENCOMP_H
#define TEXTGENCOMP_H

#include "TextGenCompImpl.h"
#include "ITextGenIfcStubHandler.h"

class TextGenComp: public YaComponent::TextGenCompImpl
                   , public YaComponent::ITextGenIfcStubHandler
{
public:
  TextGenComp(void* context);

  virtual void onRequestStartText( int id, const TextGen::Request& );
  virtual void onRequestStopText(  int id, const TextGen::Request& );
  virtual void init();

private:
  TextGenComp& self() { return *this; }
};

#endif // TEXTGENCOMP_H
