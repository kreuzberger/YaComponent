from textunittestsp import TextGenStubCallback, TextOutStubCallback, TextGenCompImpl, TextOutCompImpl
from TextGenIfc_pb2 import *
import yacomponent
import pytest
import logging
from time import sleep

class TextTestGuiEnv():
    logging.warning("right env is called")
    _context = None
    _textgen = None
    _textout = None

    def __init__(self):
        self._context = yacomponent.context_new()
        self._textgen_cb = TextGenStubCallback()
        self._textout_cb = TextOutStubCallback()
        self._textgen = TextGenCompImpl(self._context, self._textgen_cb)
        self._textout = TextOutCompImpl(self._context, self._textout_cb)
        self._textgen.init()
        self._textout.init()
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
        yacomponent.context_term(self._context)


@pytest.fixture
def textguitest():
    logging.warning("this is called")
    return TextTestGuiEnv()


def test_text_env(textguitest, qapp):
    assert textguitest._context is not None


def test_request_response_without_parameter(textguitest, qapp):
    assert textguitest._context is not None

    textguitest._textout._Xml.requestTerminate()
    sleep(0.5)
    qapp.processEvents()
    assert textguitest._textout_cb._ResponseTerminateCnt == 1

def test_request_response_with_parameter(textguitest, qapp):
    assert textguitest._context is not None
    request = Request()
    request.id = 2

    logging.info(f"sending request {request}")
    textguitest._textout._Xml.requestStartText(request)
    sleep(0.5)
    qapp.processEvents()
    assert textguitest._textout_cb._Resp_StartText.id == 2

    logging.info(f"sending request {request}")
    request.id = 4
    textguitest._textout._Xml.requestStopText(request)
    sleep(0.5)
    qapp.processEvents()
    assert textguitest._textout_cb._Resp_StopText.id == 4

def test_request_response_with_defaults(textguitest, qapp):
    assert textguitest._context is not None
    request = Request()
    request.id = 0 # this should be the "default" for numerics
    assert textguitest._textout_cb._ResponseStartCnt == 0
    assert textguitest._textout_cb._ResponseStopCnt == 0

    logging.info(f"sending request {request}")
    textguitest._textout._Xml.requestStartText(request)
    sleep(0.5)
    qapp.processEvents()
    assert textguitest._textout_cb._ResponseStartCnt == 1
    assert textguitest._textout_cb._Resp_StartText.id == 0

    logging.info(f"sending request {request}")
    request.id = 0
    textguitest._textout._Xml.requestStopText(request)
    sleep(0.5)
    qapp.processEvents()
    assert textguitest._textout_cb._ResponseStopCnt == 1
    assert textguitest._textout_cb._Resp_StopText.id == 0


def test_request_response_with_defaults_after_valid(textguitest, qapp):
    assert textguitest._context is not None
    request = Request()

    for id in [1,0]:
        request.id = id

        logging.info(f"sending request {request}")
        textguitest._textout._Xml.requestStartText(request)
        sleep(0.5)
        qapp.processEvents()
        assert textguitest._textout_cb._Resp_StartText.id == id

        logging.info(f"sending request {request}")
        request.id = id
        textguitest._textout._Xml.requestStopText(request)
        sleep(0.5)
        qapp.processEvents()
        assert textguitest._textout_cb._Resp_StopText.id == id
