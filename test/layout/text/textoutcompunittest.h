#ifndef TEXTOUTCOMPUNITTEST_H
#define TEXTOUTCOMPUNITTEST_H

#include "ITextGenIfcProxyHandler.h"
#include "TextOutCompImpl.h"

class TextOutComp : public YaComponent::TextOutCompImpl, public YaComponent::ITextGenIfcProxyHandler
{
public:
  TextOutComp( void* context );
  virtual ~TextOutComp() {}

  void onPropertyText( int, const TextGen::Text& ) override;

  void onResponseStartedText( int, const TextGen::RequestInfo& ) override;
  void onResponseStoppedText( int, const TextGen::RequestInfo& ) override;
  void onResponseTerminated( int ) override;

  void init() override;

  void setNotifications();
  void clearNotifications();

  void requestStart();
  void requestStart(int id);
  void requestStop();
  void requestStop(int id);
  void requestTerminate();

  int miPropertiesCnt;
  int miResponseStartCnt;
  TextGen::RequestInfo mResponseStartInfo;
  TextGen::RequestInfo mResponseStopInfo;

  int miResponseStopCnt;
  int miResponseTerminatedCnt;

private:
};

#endif // TEXTOUTCOMP_H
