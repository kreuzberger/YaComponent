#ifndef PUBSUBUNITTEST_MP_SLOW_SUB_H
#define PUBSUBUNITTEST_MP_SLOW_SUB_H

#include <QObject>

#include "YaComponent.h"
#include "YaComponentThread.h"
#include "publishercompunittest.h"
#include "subscriber_slow_compunittest.h"


class TextUnittestMPSub : public QObject
{
  Q_OBJECT
public:
  TextUnittestMPSub(QObject *parent = 0);
  ~TextUnittestMPSub() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testMPMTSub();

private:
    void initComponentsMPMT();
    void cleanupComponents();
    void testRoutine();

    void* mpContext;
    YaComponentThread* mpSubscriberThread;
    SubscriberComp* mpSubscriberComp;
};

#endif // TEXTUNITTEST_H
