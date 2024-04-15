#pragma once
#include <QObject>
#include <QtTest>

#include "PubSubUnittestSP_PUB.h"
#include "PubSubUnittestSP_SUB.h"

class PubSubUnittestSP : public QObject
{
    Q_OBJECT
public:
    PubSubUnittestSP(QObject *parent = 0);
    ~PubSubUnittestSP() {}
signals:

public slots:

private slots:

    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void init();
    void testConnect();
    void testSendWithoutSubscriptions();
    void testSendLVCConnect();
    void testSendLVCConnectEmpty();

private:
    void initComponents();
    void cleanupComponents();

    std::unique_ptr<PublisherComp> mPublisher = nullptr;
    std::unique_ptr<SubscriberComp> mSubScriber = nullptr;
    std::unique_ptr<YaComponentThread> mPublisherThread = nullptr;
    std::unique_ptr<YaComponentThread> mSubscriperThread = nullptr;
    void *mContext = nullptr;
};
