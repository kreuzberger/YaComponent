#ifndef TEXTOUTCOMP_H
#define TEXTOUTCOMP_H

#include "TextOutCompImpl.h"
#include "ITextGenIfcProxyHandler.h"

class TextOutComp : public YaComponent::TextOutCompImpl
                    , public ITextGenIfcProxyHandler
{
public:
  TextOutComp( void* context);

  void onProperty( int, const TextGen::Text& );

  virtual void onResponse( int, const TextGen::startedText& ) {}
  virtual void onResponse( int, const TextGen::stoppedText& ) {}

  virtual void init();

private:

  TextOutComp& self() { return *this; }
};

#endif // TEXTOUTCOMP_H
