from PublisherIfc_pb2 import *

import yacomponent
from yacomponent import Subscriber
from PublisherIfcProxy import PublisherIfcProxy
from SubscriberCompImpl import SubscriberCompImpl


from PublisherIfcProxyHandler import PublisherIfcProxyHandler

from PySide2.QtCore import QThread
from PySide2.QtTest import QTest
from PySide2.QtCore import QCoreApplication

import pytest
from time import sleep
import logging
from enum import IntEnum
import subprocess

import logging
from time import sleep
from pathlib import Path

_app_process = None



@pytest.fixture(scope="session")
def pathToPublisher(request):
    assert request.config.option.publisher, "Test cannot run without this option"
    return Path(request.config.option.publisher).resolve()



def _start_publisher(cmd: str):
    logging.debug(f"start app {cmd}")

    args = [cmd, "--verbose"]

    logging.info(args)
    global _app_process
    _app_process = subprocess.Popen(args)

def _check_process(process: subprocess.Popen):
    logging.debug(f"process {process.args} running")
    # print("#", end="")
    proc_running = process.poll()
    if proc_running is None:
        return True
    else:
        logging.info(f"process {process.args} terminated with return code {proc_running}")
        return False
    return False


def _terminate():
    for process in [_app_process]:
        if process is not None:
            process.terminate()
            process.wait(1)
            process.kill()


class PublisherProxyCallback(PublisherIfcProxyHandler):

    _last_data = None
    _last_time = None
    _properties_cnt = 0

    def __init__(self):
        self._properties_cnt = 0
        self._last_data = None
        self._last_time = None

    def onPropertyData(self, id: int, property: Data):
        logging.debug(f"PublisherProxyCallback::onProperty Data:")
        self._last_data = property
        self._properties_cnt += 1

    def onPropertyTime(self, id: int, property: Time ):
        self._last_time = property
        self._properties_cnt += 1
        logging.debug(f"PublisherProxyCallback::onProperty Time:")


class PubSubEnv():
    _context = None
    _sub_thread = None
    _sub = None

    def __init__(self, request):
        self._context = yacomponent.context_new()
        self._sub_thread = QThread()
        self._sub_cb = PublisherProxyCallback()
        self._sub = SubscriberCompImpl(self._context, self._sub_cb)
        self._sub_thread.start()
        self._sub.init()
        self._sub.moveToThread(self._sub_thread)

        _start_publisher(request.config.option.publisher)
        # logging.info("setConnectionParaReceiverData")
        # self._pub.setConnectionParaReceiverData("inproc://pytestingprocsubpub", 3000);
        logging.info("setConnectionParaData")
        self._sub.setConnectionParaData("tcp://127.0.0.1:42165", "pysubscriber_mp");
        logging.info("init finished")


    def __del__(self):
        logging.info("terminate")
        self._sub.close()
        self._sub_thread.quit()
        logging.info("terminate wait for Thread finished")
        self._sub_thread.wait()
        logging.info("terminate Thread finished")
        sleep(0.1)
        assert( self._sub_thread.isFinished())
        yacomponent.context_term(self._context)
        _terminate()


@pytest.fixture
def pubsubtest(request):
    return PubSubEnv(request)


def test_pubsub_env(pubsubtest, qapp):
    assert pubsubtest._context is not None
    assert pathToPublisher is not None
    _check_process(_app_process)

def test_notifications(pubsubtest, qapp):
    rc = pubsubtest._sub._Data.setNotification(PublisherIfcProxy.KEYS.PROP_DATA);
    assert -1 != rc
    sleep(0.05)
    rc = pubsubtest._sub._Data.requestStartData()

    sleep(2)

    logging.info(f"test_notifications: received {pubsubtest._sub_cb._properties_cnt} property updates")
    assert 0 < pubsubtest._sub_cb._last_data.counter
    assert 0 < pubsubtest._sub_cb._properties_cnt

    logging.info(f"test_notifications: stopping data")
    rc = pubsubtest._sub._Data.requestStopData()

    pubsubtest._sub_cb._last_time = None
    pubsubtest._sub_cb._properties_cnt = 0

    sleep(2)

    assert pubsubtest._sub_cb._last_time is None
    assert pubsubtest._sub_cb._properties_cnt == 0




