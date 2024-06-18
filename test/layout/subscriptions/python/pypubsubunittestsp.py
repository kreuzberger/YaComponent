import PublisherIfc_pb2

import yacomponent
#from yacomponent.subscriber import Subscriber
#from yacomponent.publisher import Publisher
from yacomponent import Subscriber
from yacomponent import Publisher
import PublisherIfcProxy

from PySide2.QtCore import QThread
from PySide2.QtTest import QTest
import pytest
from time import sleep
import logging
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

class PubSubEnv():
    _context = None
    _pub_thread = None
    _sub_thread = None
    _pub = None
    _sub = None


    def __init__(self):
        self._context = yacomponent.context_new()
        self._pub_thread = yacomponent.Thread()
        self._sub_thread = yacomponent.Thread()
        self._pub = yacomponent.Stub(self._context,1)
        self._sub = yacomponent.Proxy(self._context,2)
        self._pub_thread.start()
        self._sub_thread.start()
        self._sub.setConnectionPara("inproc://pytestingprocsubpub", "pysubscriber_sp");
        self._pub.setConnectionPara("inproc://pytestingprocsubpub", 3000);

    def __del__(self):
        self._pub.close()
        self._sub.close()
        self._pub_thread.quit()
        self._sub_thread.quit()
        sleep(0.1)
        assert( self._pub_thread.isFinished())
        assert( self._sub_thread.isFinished())
        yacomponent.context_term(self._context)


@pytest.fixture
def pubsubtest():
    return PubSubEnv()


def test_protobuf():
    data = PublisherIfc_pb2.Data()
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

def test_pubsub_env(pubsubtest):
    assert pubsubtest._context is not None

def test_basic_notifications(pubsubtest, qapp):
    rc = pubsubtest._sub.setNotification(0)
    assert -1 != rc
    sleep(0.05)
    msg = "this is the data".encode("utf-8")
    rc = pubsubtest._pub.send(0, len(msg), msg)
    print(f"this is rc {rc}")
    assert -1 != rc


