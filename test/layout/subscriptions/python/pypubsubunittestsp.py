from PublisherIfc_pb2 import *

import yacomponent
#from yacomponent.subscriber import Subscriber
#from yacomponent.publisher import Publisher
from yacomponent import Subscriber
from yacomponent import Publisher
from PublisherIfcProxy import PublisherIfcProxy
from PublisherIfcStub import PublisherIfcStub
from SubscriberCompImpl import SubscriberCompImpl
from PublisherCompImpl import PublisherCompImpl


from PublisherIfcProxyHandler import PublisherIfcProxyHandler
from PublisherIfcStubHandler import PublisherIfcStubHandler

from PySide2.QtCore import QThread
from PySide2.QtTest import QTest
from PySide2.QtCore import QCoreApplication

import pytest
from time import sleep
import logging
from enum import IntEnum
#from pytestqt.qtbot import QtBot

# todo
# create a context that does this similar to cpp
# mContext = YaComponent::context_new();
# mPublisherThread = std::make_unique<YaComponentThread>();
# mSubscriperThread = std::make_unique<YaComponentThread>();
# mSubScriber.reset(new SubscriberComp(mContext));
# mPublisher.reset(new PublisherComp(mContext));

# mPublisherThread->start();
# mSubscriperThread->start();
# QTest::qWait(10);
# mSubScriber->init();
# mPublisher->init();
# mPublisher->setConnectionParaReceiverData("inproc://testinprocsubpub", 3000);

# mSubScriber->setConnectionParaData("inproc://testinprocsubpub", "subscriber_sp");
# mPublisher->moveToThread(mPublisherThread.get());
# mSubScriber->moveToThread(mSubscriperThread.get());

import logging
from time import sleep



class PublisherProxyCallback(PublisherIfcProxyHandler):

    _last_data = None
    _last_time = None
    _properties_cnt = 0

    def onPropertyData(self, id: int, property: Data):
        logging.info(f"PublisherProxyCallback::onProperty Data:")
        self._last_data = property
        self._properties_cnt += 1

    def onPropertyTime(self, id: int, property: Time ):
        self._last_time = property
        self._properties_cnt += 1
        logging.info(f"PublisherProxyCallback::onProperty Time:")

class PublisherStubCallback(PublisherIfcStubHandler):

    def onRequestStartData(self, id: int  ):
        logging.info(f"PublisherStubCallback::onRequestStartData {id}")


    def onRequestStopData(self, id: int  ):
        logging.info(f"PublisherStubCallback::onRequestStopData {id}")
        pass

class PubSubEnv():
    _context = None
    _pub_thread = None
    _sub_thread = None
    _pub = None
    _sub = None

    def __init__(self):
        self._context = yacomponent.context_new()
        self._pub_thread = QThread()
        self._sub_thread = QThread()
        self._pub_cb = PublisherStubCallback()
        self._sub_cb = PublisherProxyCallback()
        self._pub = PublisherCompImpl(self._context, self._pub_cb)
        self._sub = SubscriberCompImpl(self._context, self._sub_cb)
        self._pub_thread.start()
        self._sub_thread.start()
        self._pub.init()
        self._sub.init()
        self._pub.moveToThread(self._pub_thread)
        self._sub.moveToThread(self._sub_thread)
        logging.info("setConnectionParaReceiverData")
        self._pub.setConnectionParaReceiverData("inproc://pytestingprocsubpub", 3000);
        logging.info("setConnectionParaData")
        self._sub.setConnectionParaData("inproc://pytestingprocsubpub", "pysubscriber_sp");
        logging.info("init finished")


    def __del__(self):
        logging.info("terminate")
        self._pub.close()
        self._sub.close()
        self._pub_thread.quit()
        self._sub_thread.quit()
        logging.info("terminate wait for Thread finished")
        self._pub_thread.wait()
        self._sub_thread.wait()
        logging.info("terminate Thread finished")
        sleep(0.1)
        assert( self._pub_thread.isFinished())
        assert( self._sub_thread.isFinished())
        yacomponent.context_term(self._context)


@pytest.fixture
def pubsubtest():
    return PubSubEnv()


def test_protobuf():
    data = Data()
    data.timeSeconds = 12345
    data.counter = 10
    assert True

def test_zmq():
    ctx = yacomponent.context_new()
    yacomponent.context_term(ctx)
    assert True

def test_base_impls(qapp):
    ctx = yacomponent.context_new()
    proxy = yacomponent.Proxy(ctx, 1)
    stub = yacomponent.Stub(ctx, 2)
    yacomponent.context_term(ctx)
    pub_thread = yacomponent.Thread
    sub_thread = yacomponent.Thread

    assert True

def test_pubsub_env(pubsubtest, qapp):
    assert pubsubtest._context is not None

def test_basic_notifications(pubsubtest, qapp):
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    assert -1 != rc
    sleep(0.05)

    data = Data()
    #logging.info(f"data {dir(data)}")
    data.timeSeconds = 0
    data.timeFraction = 0.0
    data.counter = 123

    rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
    sleep(1)

    assert 123 == pubsubtest._sub_cb._last_data.counter

    # QCOMPARE(mSubScriber->miPropertiesCnt, 1);
    # QCOMPARE(mSubScriber->mLastData.counter(), 123);

