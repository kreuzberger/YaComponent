import zmq
from .subscriber import Subscriber

from PySide2.QtCore import QObject

class Proxy(QObject):

    _id = 0
    _Subscriber = None

    def __init__(self, ctx: zmq.Context, id:int):
        self._Subscriber = Subscriber(ctx)
        self._id = id

    def setNotification( self, key: int) -> int:
        return self._Subscriber.setNotification(key)

    def clearNotification( self, key: int) -> int:
        return self._Subscriber.clearNotification(key)

    def setConnectionPara(self, address: str, ident: str):
        self._Subscriber.setConnectionPara(address, ident)

    def close(self):
        self._Subscriber.close()
