#ifndef TEXTOUTCOMP_H
#define TEXTOUTCOMP_H

#include "TextOutCompImpl.h"
#include "ITextGenIfcProxyHandler.h"

class TextOutComp : public YaComponent::TextOutCompImpl
                    , public ITextGenIfcProxyHandler
{
public:
  TextOutComp( void* context);

  void onProperty( const YaProxyBase*, const TextGen::Text& ) {}

  virtual void onResponse( const YaProxyBase*, const TextGen::startedText& ) {}
  virtual void onResponse( const YaProxyBase*, const TextGen::stoppedText& ) {}

private:

  TextOutComp& self() { return *this; }
};

#endif // TEXTOUTCOMP_H
