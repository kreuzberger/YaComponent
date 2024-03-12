#pragma once
#include <QObject>

#include "textgencompunittest.h"
#include <yacomponent/YaComponent.h>
#include <yacomponent/YaComponentThread.h>

class TextUnittestMPPub : public QObject
{
    Q_OBJECT
public:
    TextUnittestMPPub(QObject *parent = 0);
    ~TextUnittestMPPub() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testMPMTPub();

private:
    void initComponentsMPMT();
    void cleanupComponents();
    void testRoutine();

    void *mpContext;
    YaComponentThread *mpTextGenThread;
    TextGenComp *mpTextGen1;
};
