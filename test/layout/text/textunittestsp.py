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



class TextTestEnv():
    _context = None
    _pub_thread = None
    _sub_thread = None
    _pub = None
    _sub = None

    def __init__(self):
        self._context = yacomponent.context_new()


    def __del__(self):
        yacomponent.context_term(self._context)


@pytest.fixture
def texttest():
    return TextTestEnv()



# def test_base_impls(qapp):
#     ctx = yacomponent.context_new()
#     proxy = yacomponent.Proxy(ctx, 1)
#     stub = yacomponent.Stub(ctx, 2)
#     yacomponent.context_term(ctx)
#     pub_thread = QThread
#     sub_thread = QThread

#     assert True

def test_text_env(texttest, qapp):
    assert texttest._context is not None

