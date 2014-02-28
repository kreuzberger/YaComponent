#ifndef TEXTOUTCOMP_H
#define TEXTOUTCOMP_H

#include "TextOutCompImpl.h"
#include "TextGenIfcProxy.h"

class TextOutComp : public YaComponent::TextOutCompImpl
                    , public YaComponent::TextGenIfcProxy
{
public:
  TextOutComp();

protected:
  virtual void onProperty(const TextGen::Text&) {}
  virtual void onResponse(const TextGen::startedText&) {};
  virtual void onResponse(const TextGen::stoppedText&) {};
};

#endif // TEXTOUTCOMP_H
