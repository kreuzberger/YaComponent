#pragma once
#include <QObject>

#include "textgencompunittest.h"
#include "textoutcompunittest.h"
#include <yacomponent/YaComponent.h>
#include <QtCore/QThread>

class TextUnittest : public QObject
{
    Q_OBJECT
public:
    TextUnittest(QObject *parent = 0);
    ~TextUnittest() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testSPST();
    void testSPMT();
    void testSPMTGui();
    void testSPMTResponse();
    void testSPMTAllMessages();

private:
    void initComponentsSPST();
    void initComponentsSPMT();
    void initComponentsSPMTGui();
    void cleanupComponents();
    void testRoutine();

    void *mpContext;
    QThread *mpTextGenThread;
    TextGenComp *mpTextGen1;
    QThread *mpTextOutThread;
    void *mpContext2;
    TextOutComp *mpTextOut;
};

