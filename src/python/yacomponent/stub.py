import zmq
from .publisher import Publisher
from PySide2.QtCore import QObject

# equivalent class to YaStubBase

class Stub(QObject):

    _id = 0
    _Publisher = None

    def __init__(self, ctx: zmq.Context, id:int):
        self._Publisher = Publisher(ctx)
        self._id = id

    def send( self, key: int, size: int, msg) -> int:
        return self._Publisher.send(key,size,msg)

    def setConnectionPara(self, address: str, hwm: int):
        # hwm = High Water Mark
        self._Publisher.setConnectionPara(address, hwm)

    def close(self):
        self._Publisher.close()
