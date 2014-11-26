#ifndef SUBSCRIBER_FAST_COMPUNITTEST_H
#define SUBSCRIBER_FAST_COMPUNITTEST_H

#include "SubscriberCompImpl.h"
#include "IPublisherIfcProxyHandler.h"

class SubscriberComp : public YaComponent::SubscriberCompImpl
                    , public IPublisherIfcProxyHandler
{
public:
  SubscriberComp( void* context);
  virtual ~SubscriberComp() {}

  void onProperty( int, const Data& );


  virtual void init();

  int setNotifications();
  int clearNotifications();

  int requestStart();
  int requestStop();


  int miPropertiesCnt;

private:

  SubscriberComp& self() { return *this; }
};

#endif // TEXTOUTCOMP_H
