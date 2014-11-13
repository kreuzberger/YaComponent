#ifndef TEXTUNITTEST_MP_SUB_H
#define TEXTUNITTEST_MP_SUB_H

#include <QObject>

#include "YaComponent.h"
#include "YaComponentThread.h"
#include "textgencompunittest.h"
#include "textoutcompunittest.h"


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
    YaComponentThread* mpTextOutThread;
    TextOutComp* mpTextOut;
};

#endif // TEXTUNITTEST_H
