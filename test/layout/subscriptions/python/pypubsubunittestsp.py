from PublisherIfc_pb2 import *

import yacomponent
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

import logging
from time import sleep



class PublisherProxyCallback(PublisherIfcProxyHandler):

    _last_data = None
    _last_time = None
    _properties_cnt = 0

    def __init__(self):
        self._properties_cnt = 0
        self._last_data = None
        self._last_time = None

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

# def test_base_impls(qapp):
#     ctx = yacomponent.context_new()
#     proxy = yacomponent.Proxy(ctx, 1)
#     stub = yacomponent.Stub(ctx, 2)
#     yacomponent.context_term(ctx)
#     pub_thread = QThread
#     sub_thread = QThread

#     assert True

def test_pubsub_env(pubsubtest, qapp):
    assert pubsubtest._context is not None

def test_notifications(pubsubtest, qapp):
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    assert -1 != rc
    sleep(0.05)

    data = Data()
    #logging.info(f"data {dir(data)}")
    data.timeSeconds = 0
    data.timeFraction = 0.0
    data.counter = 123

    rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
    sleep(0.05)

    assert 123 == pubsubtest._sub_cb._last_data.counter
    assert 1 == pubsubtest._sub_cb._properties_cnt

def test_send_without_subscriptions(pubsubtest, qapp):
    data = Data()
    data.timeSeconds = 0
    data.timeFraction = 0.0
    data.counter = 123
    rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
    sleep(0.05)
    assert -1 == rc
    assert 0 == pubsubtest._sub_cb._properties_cnt


def test_send_lvc_connect(pubsubtest, qapp):
    for i in range(10):
        data = Data()
        data.timeSeconds = 0
        data.timeFraction = 0.0
        data.counter = i
        rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
        assert -1 == rc
        sleep(0.05)

    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    assert -1 != rc
    sleep(0.05)
    assert 1 == pubsubtest._sub_cb._properties_cnt
    assert 9 == pubsubtest._sub_cb._last_data.counter


def test_send_lvc_connect_empty(pubsubtest, qapp):
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    assert -1 != rc
    sleep(0.05)
    assert 0 == pubsubtest._sub_cb._properties_cnt
    assert pubsubtest._sub_cb._last_data is None


def test_send_on_change_off(pubsubtest, qapp):
    # send identical value twice, should be received a second time
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    sleep(0.05)

    data = Data()
    data.timeSeconds = 0
    data.timeFraction = 0.0
    data.counter = 123

    rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
    sleep(0.05)

    assert 123 == pubsubtest._sub_cb._last_data.counter
    assert 1 == pubsubtest._sub_cb._properties_cnt

    pubsubtest._sub_cb._last_data = None
    rc = pubsubtest._pub._ReceiverData.sendData(PublisherIfcStub.KEYS.PROP_DATA, data)
    sleep(0.05)

    assert 123 == pubsubtest._sub_cb._last_data.counter
    assert 2 == pubsubtest._sub_cb._properties_cnt


def test_send_on_change_on(pubsubtest, qapp):
    # send identical value twice, should be received a second time
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_TIME);
    sleep(0.05)

    time = Time()
    time.timeSeconds = 0
    time.timeFraction = 0.0
    time.counter = 321

    rc = pubsubtest._pub._ReceiverData.sendTime(PublisherIfcStub.KEYS.PROP_TIME, time)
    sleep(0.05)

    assert 321 == pubsubtest._sub_cb._last_time.counter
    assert 1 == pubsubtest._sub_cb._properties_cnt

    pubsubtest._sub_cb._last_time = None
    pubsubtest._sub_cb._properties_cnt = 0
    rc = pubsubtest._pub._ReceiverData.sendTime(PublisherIfcStub.KEYS.PROP_TIME, time)
    sleep(0.05)

    assert pubsubtest._sub_cb._last_data is None
    assert 0 == pubsubtest._sub_cb._properties_cnt
