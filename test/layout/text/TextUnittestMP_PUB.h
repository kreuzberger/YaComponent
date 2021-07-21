#ifndef TEXTUNITTESTMP_PUB_H
#define TEXTUNITTESTMP_PUB_H

#include <QObject>

#include "YaComponent.h"
#include "YaComponentThread.h"
#include "textgencompunittest.h"

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

#endif // TEXTUNITTEST_H
