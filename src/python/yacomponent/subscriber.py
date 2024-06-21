import zmq
from . import variables as yc

from google.protobuf.message import Message
from time import sleep
#from PySide2.QtCore import QCoreApplication

import logging

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
            logging.info(f"Subscriber::setConnectionPara: connecting to {address}")
            self._reqresp_socket.connect(address)
            logging.info(f"Subscriber::setConnectionPara: connected to {address}")
            self._connected = True
            key = yc.KeyFmt % yc.KeySync

            rc = self._reqresp_socket.send_string("",zmq.Flag.SNDMORE);
            self._reqresp_socket.send_string(key,0)
            sleep(yc.TimeOut)
            sync = False
            while not sync:
                #logging.info("Subscriber::setConnectionPara: syncing")
                #QCoreApplication.processEvents()
                sleep(yc.TimeOut)
                sync = self._checkSync()
        else:
            raise RuntimeError("no valid socket or not connected")

    def send(self, key: int, msgSize: int, data: bytes)-> int:
        rc = -1;
        if self._reqresp_socket is None:
            raise RuntimerError("_reqresp_socket_should not be none")
        if self._reqresp_socket is not None:
             rc = self._reqresp_socket.send(b"",zmq.Flag.SNDMORE);
             assert -1 != rc

        send_more = True if 0 != msgSize and data is not None else False
        flags = zmq.SNDMORE if send_more else 0
        rc = self._reqresp_socket.send((yc.KeyFmt % key).encode("ascii"), flags);
        assert -1 != rc;
        if send_more:
            rc = self._reqresp_socket.send(data.encode("utf-8"), 0)

        return rc;


    #def receive(self, key: int, size: int, data , ident: str) -> int:
    def receive(self) -> tuple[int,  bytes]:
        bytes = 0
        more = -1
        key = None
        msg = None
        assert self._reqresp_socket is not None
        if self._reqresp_socket is not None:
            rc = self._reqresp_socket.poll(0, zmq.PollEvent.POLLIN)

            if rc == zmq.PollEvent.POLLIN:
                #logging.info(f"Subscriber::receive pollin")
                msg = self._reqresp_socket.recv(zmq.NOBLOCK)
                #logging.info(f"Subscriber::receive msg {msg}")
                msg = self._reqresp_socket.recv(zmq.NOBLOCK)
                key = int(msg)
                #logging.info(f"Subscriber::receive key {key}")
                if 0 <= key:
                    more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                    if more:
                        msg = self._reqresp_socket.recv(zmq.NOBLOCK);
                        logging.info(f"Subscriber::receive key {key} msg {msg}")
                        more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                        if more:
                            raise RuntimerError("Subscriber::receive: KeyFmt unexpected end of message");
                        # logging.info(f"Subscriber::receive 3")

                elif yc.KeySync == key:
                    logging.info(f"Subscriber::receive keySync")
                    key = None # returning key should not be handled by caller
                    more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                    if more:
                        msg = self._reqresp_socket.recv(zmq.NOBLOCK);
                        logging.info(f"Subscriber::receive keySync msg {msg}")
                        if 0 < len(msg):
                            self._sync = True if yc.SynAck == msg.decode("ascii") else False
                            logging.info(f"Subscriber::receive keySync synced {yc.SynAck} {msg} {self._sync}")
                            more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                            if  more:
                                raise RuntimeError("Subscriber::receive: KeySync unexpected end of message");
                elif yc.KeyEnd == key:
                    key = None # returning key should not be handled by caller
                    more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                    if more:
                        raise RuntimeError("Subscriber::receive: KeyEnd unexpected end of message");
                else:
                    raise RuntimeError(f"Subscriber::receive received invalid key {key}")

        #logging.info(f"Subscriber::receive returns with {key}, {msg}")
        return (key, msg)

    def request(self, key: int, msg: Message )-> int: #const ::google::protobuf::MessageLite
        rc = -1;
        size = msg.ByteSize();
        msg.SerializeToString(self._msg_outbuffer);
        rc = self.send(key, size, self._msg_outbuffer);
        return rc;

    def request(self, key: int)->int:
        return self.send(key, 0, 0);

    def setNotification(self, key: int)->int:
        rc = -1
        msg = yc.KeyFmt % (key)
        rc  = self.send(yc.KeySetNotification, yc.KeySize, msg)
        return rc

    def clearNotification(self, key: int)->int:
        msg = yc.KeyFmt % (key)
        self.send(yc.KeyClearNotification, yc.KeySize, msg)
        return -1


    def close(self):
        self._reqresp_socket.close()
        self._reqresp_socket = None


    def _checkSync(self) -> bool:
        return self._sync
