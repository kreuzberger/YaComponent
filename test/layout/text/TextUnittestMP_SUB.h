#pragma once

#include <QObject>

#include "textoutcompunittest.h"
#include <yacomponent/YaComponent.h>
#include <QtCore/QThread>

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
    QThread *mpTextOutThread;
    TextOutComp *mpTextOut;
};
