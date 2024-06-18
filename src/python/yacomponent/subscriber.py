import zmq
from . import variables as yc

from google.protobuf.message import Message
from time import sleep
from PySide2.QtCore import QCoreApplication

class Subscriber():

    _reqresp_socket = None
    _ident = None
    _msg_outbuffer = ''
    _connected = False
    _sync = False

    def __init__(self, ctx):
        if ctx is None:
            raise RuntimeError("invalid context, aquire zmq context and pass")
        self._reqresp_socket = ctx.socket(zmq.DEALER)
        self._ident = None
        self._connected = False
        self._sync = False

    def setConnectionPara(self, address: str, ident: bytes):
        assert self._reqresp_socket is not None
        assert address is not None and 0 < len(address)
        assert ident is not None and len(ident) < yc.MaxIdentSize
        self._ident = ident
        if self._reqresp_socket is not None and not self._connected:
            self._reqresp_socket.setsockopt_string(zmq.IDENTITY,ident)
            self._reqresp_socket.setsockopt(zmq.RCVTIMEO, 0)
            self._reqresp_socket.connect(address)
            self._connected = True
            key = yc.KeyFmt % yc.KeySync
            rc = self._reqresp_socket.send_string("",zmq.Flag.SNDMORE);
            assert -1 != rc
            self._reqresp_socket.send_string(key,0)
            assert -1 != rc
            sleep(yc.TimeOut)
            sync = False
            while not sync:
                QCoreApplication.processEvents()
                sync = checkSync()
        else:
            raise RuntimeError("no valid socket or not connected")

    def send(self, key: int, msgSize: int, data: bytes)-> int:
        rc = -1;
        raise RuntimerError("_reqresp_socket_should not be none") if _reqresp_socket is None else None
        if self._reqresp_socket is not None:
             rc = self._reqresp_socket.send("",zmq.Flag.SNDMORE);
             assert -1 != rc

        send_more = True if 0 != size and data is not None else False
        flags = zmq.SNDMORE if send_more else 0
        ckey = yc.KeyFmt % key
        rc = mpReqRespSocket.send(ckey, flags);
        assert -1 != rc;
        if send_more:
            rc = mpReqRespSocket.send(data, 0)

        return rc;


    def receive(self, key: int, size: int, data , ident: str) -> int:
        bytes = 0
        more = -1
        assert self._reqresp_socket is not None
        if self._reqresp_socket is not None:
            rc = self._reqresp_socket.poll(None, zmq.PollEvent.POLLIN)

            if rc == zmq.PollEvent.POLLIN:
                msg = self._reqresp_socket.rcv(zmq.NOBLOCK)
                assert 0 == len(msg);
                msg = self._reqresp_socket.rcv(zmq.NOBLOCK)
                assert len(msg) == yc.KeySize
                key = int(msg)
                if 0 <= key:
                    more = self._reqresp_socket.getsockopt(ZMQ_RCVMORE)
                    if more:
                        msg = self._reqresp_socket.recv(ZMQ_NOBLOCK);
                        more = self._reqresp_socket.getsockopt(ZMQ_RCVMORE)
                        if more and 0 < len(msg):
                            raise RuntimerError("Subscriber::receive: KeyFmt unexpected end of message");
                elif yc.KeySync == key:
                    iBytes = 0;
                    more = self._reqresp_socket.getsockopt(ZMQ_RCVMORE)
                    if more:
                        msg = self._reqresp_socket.recv(ZMQ_NOBLOCK);
                        if 0 < len(msg):
                            self._sync = True if yc.SyncAck == msg else False
                            more = self._reqresp_socket.getsockopt(ZMQ_RCVMORE)
                            if  more:
                                raise RuntimeError("Subscriber::receive: KeySync unexpected end of message");
                elif yc.KeyEnd == key:
                    iBytes = 0;
                    more = self._reqresp_socket.getsockopt(ZMQ_RCVMORE)
                    if more:
                        raise RuntimeError("Subscriber::receive: KeyEnd unexpected end of message");
                else:
                    raise RuntimeError(f"Subscriber::receive received invalid key {key}")

        return bytes

    def request(self, key: int, msg: Message )-> int: #const ::google::protobuf::MessageLite
        rc = -1;
        size = msg.ByteSize();
        msg.SerializeToString(self._msg_outbuffer);
        rc = send(key, isze, self._msg_outbuffer);
        return rc;

    def request(self, key: int)->int:
        return send(key, 0, 0);

    def setNotification(self, key: int)->int:
        rc = -1
        msg = yc.KeyFmt % (key)
        rc  = send(yc.KeySetNotification, yc.KeySize, msg)
        return rc

    def clearNotification(self, key: int)->int:
        msg = yc.KeyFmt % (key)
        send(yc.KeyClearNotification, yc.KeySize, msg)
        return -1


    def close(self):
        _reqresp_socket.close()
        _reqresp_socket = None


    def checkSync(self) -> bool:
        return self._sync
