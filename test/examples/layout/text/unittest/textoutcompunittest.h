#ifndef TEXTOUTCOMPUNITTEST_H
#define TEXTOUTCOMPUNITTEST_H

#include "TextOutCompImpl.h"
#include "ITextGenIfcProxyHandler.h"

class TextOutComp : public YaComponent::TextOutCompImpl
                    , public ITextGenIfcProxyHandler
{
public:
  TextOutComp( void* context);

  void onProperty( int, const TextGen::Text& );

  virtual void onResponse( int, const TextGen::startedText& ) { miResponseStartCnt++; }
  virtual void onResponse( int, const TextGen::stoppedText& ) { miResponseStopCnt++; }

  virtual void init();

  void setNotifications();
  void clearNotifications();

  void requestStart();
  void requestStop();


  int miPropertiesCnt;
  int miResponseStartCnt;
  int miResponseStopCnt;

private:

  TextOutComp& self() { return *this; }
};

#endif // TEXTOUTCOMP_H
