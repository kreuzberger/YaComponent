#pragma once

#include <QObject>

#include <yacomponent/YaComponent.h>
#include <yacomponent/YaComponentThread.h>
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

    void *mpContext;
    YaComponentThread *mpTextOutThread;
    TextOutComp *mpTextOut;
};
