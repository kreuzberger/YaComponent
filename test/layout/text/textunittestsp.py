from TextGenIfc_pb2 import *

import yacomponent
from TextGenIfcProxy import TextGenIfcProxy
from TextGenIfcStub import TextGenIfcStub
from TextOutIfcProxy import TextOutIfcProxy
from TextOutIfcStub import TextOutIfcStub
from TextOutCompImpl import TextOutCompImpl
from TextGenCompImpl import TextGenCompImpl


from TextGenIfcProxyHandler import TextGenIfcProxyHandler
from TextGenIfcStubHandler import TextGenIfcStubHandler

#from TextOutIfcProxyHandler import TextOutIfcProxyHandler
#from TextOutIfcStubHandler import TextOutIfcStubHandler

from PySide2.QtCore import QThread
from PySide2.QtTest import QTest
from PySide2.QtCore import QCoreApplication

import pytest
from time import sleep
import logging
from enum import IntEnum

import logging
from time import sleep


class TextOutStubCallback(TextGenIfcProxyHandler):
    _ResponseStartCnt = 0
    _ResponseStopCnt = 0
    _ResponseTerminateCnt = 0
    _Resp_StartText = None
    _Resp_StopText = None

    def __init__(self):
        self._ResponseStartCnt = 0
        self._ResponseTerminateCnt = 0

    def onPropertyText(self, id: int, property: Text ):
        logging.info(f"TextOutStubCallback::onPropertyText")


    def onResponseStartedText( self, proxyId: int, msg: RequestInfo):
        logging.info(f"TextOutStubCallback::onResponseStartedText {RequestInfo}")
        assert(TextOutCompImpl.PROXY_IDS.XML == proxyId)
        self._ResponseStartCnt += 1
        self._Resp_StartText = msg

    def onResponseStoppedText( self, proxyId: int, msg: RequestInfo):
        logging.info(f"TextOutStubCallback::onResponseStoppedText {RequestInfo}")
        self._ResponseStopCnt += 1
        self._Resp_StopText = msg


    def onResponseTerminated( self, proxyId: int):
        logging.info(f"TextOutStubCallback::onResponseTerminated")
        self._ResponseTerminateCnt += 1


class TextGenStubCallback(TextGenIfcStubHandler):

    def onRequestStartText(self, id: int , Request, RequestInfo ):
        logging.info(f"TextGenStubCallback::onRequestStartText Request: {Request}, RequestInfo {RequestInfo}")
        RequestInfo.id = Request.id


    def onRequestStopText(self, id: int , Request, RequestInfo ):
        logging.info(f"TextGenStubCallback::onRequestStopText  Request: {Request}, RequestInfo {RequestInfo}")
        RequestInfo.id = Request.id


    def onRequestTerminate(self, id: int  ):
        logging.info(f"TextGenStubCallback::onRequestTerminate")



class TextTestEnv():
    _context = None
    _textgen_thread = None
    _textout_thread = None
    _textgen = None
    _textout = None

    def __init__(self):
        self._context = yacomponent.context_new()
        self._textgen_thread = QThread()
        self._textout_thread = QThread()
        self._textgen_cb = TextGenStubCallback()
        self._textout_cb = TextOutStubCallback()
        self._textgen = TextGenCompImpl(self._context, self._textgen_cb)
        self._textout = TextOutCompImpl(self._context, self._textout_cb)
        self._textgen_thread.start()
        self._textout_thread.start()
        self._textgen.init()
        self._textout.init()
        self._textgen.moveToThread(self._textgen_thread)
        self._textout.moveToThread(self._textout_thread)
        logging.info("setConnectionParaTextGenXml")
        self._textgen.setConnectionParaTextGenXml("inproc://pyhurtmexml", 3000);
        self._textgen.setConnectionParaTextGenPlain("inproc://pyhurtmeplain", 3000);
        logging.info("setConnectionParaXml")
        self._textout.setConnectionParaXml("inproc://pyhurtmexml", "TextOut");
        logging.info("init finished")



    def __del__(self):
        logging.info("terminate")
        self._textgen.close()
        self._textout.close()
        self._textgen_thread.quit()
        self._textout_thread.quit()
        logging.info("terminate wait for Thread finished")
        self._textgen_thread.wait()
        self._textout_thread.wait()
        logging.info("terminate Thread finished")
        sleep(0.1)
        assert( self._textgen_thread.isFinished())
        assert( self._textout_thread.isFinished())
        yacomponent.context_term(self._context)


@pytest.fixture
def texttest():
    return TextTestEnv()


def test_text_env(texttest, qapp):
    assert texttest._context is not None


def test_request_response_without_parameter(texttest, qapp):
    assert texttest._context is not None

    texttest._textout._Xml.requestTerminate()
    sleep(0.5)
    assert texttest._textout_cb._ResponseTerminateCnt == 1

def test_request_response_with_parameter(texttest, qapp):
    assert texttest._context is not None
    request = Request()
    request.id = 2

    logging.info(f"sending request {request}")
    texttest._textout._Xml.requestStartText(request)
    sleep(0.5)
    assert texttest._textout_cb._Resp_StartText.id == 2

    logging.info(f"sending request {request}")
    request.id = 4
    texttest._textout._Xml.requestStopText(request)
    sleep(0.5)
    assert texttest._textout_cb._Resp_StopText.id == 4


