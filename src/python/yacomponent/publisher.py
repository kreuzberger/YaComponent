import zmq
from . import variables as yc

from google.protobuf.message import Message
from time import sleep
from PySide2.QtCore import QCoreApplication

import logging

class Publisher():

    _reqresp_socket = None
    _bound = False
    _subscriber_count = 0
    _msg_outbuffer = ''
    _peer_map = {}
    _lvc_map = {}

    def __init__(self, ctx):
        if ctx is None:
            raise RuntimeError("invalid context, aquire zmq context and pass")
        self._reqresp_socket = ctx.socket(zmq.ROUTER)

    def setConnectionPara(self, address: str, hwm: int = 0):
        assert self._reqresp_socket is not None
        assert address is not None and 0 < len(address)

        if self._reqresp_socket is not None and not self._bound:
            if 0 < hwm:
                self._reqresp_socket.setsockopt(zmq.SNDHWM, hwm);
                self._reqresp_socket.setsockopt(zmq.SNDTIMEO, 0)

            self._reqresp_socket.setsockopt(zmq.ROUTER_MANDATORY,1);

            if self._reqresp_socket.bind(address):
                self._bound = True;
                logging.info(f"Publisher::setConnectionPara: bound to {address}")
            else:
                raise RuntimeError(f"error: cannot bind to address {address}")
        else:
            if self._bound:
                raise RuntimeError("already bound")
            if not self._reqresp_socket:
                raise RuntimeError("no socket!!")


    def response(self, key: int, msg, ident: str)-> int: # const ::google::protobuf::MessageLite
        rc = -1
        size = msg.ByteSize()
        msg.SerializeToString(self._msg_outbuffer)
        rc = self._send_ident(key, size, self._msg_outbuffer, ident)
        return rc


    def send(self, key: int, msg: Message)-> int: #const ::google::protobuf::MessageLite
        rc = -1
        size = msg.ByteSize()
        msg.SerializeToString(self._msg_outbuffer)
        rc = self._send(key, size, self._msg_outbuffer)
        return rc



    def _send(self, key: int, msgSize: int, msgData: bytes)-> int:
        rc = -1;
        if self._reqresp_socket is None:
            raise RuntimerError("_reqresp_socket_should not be none")
        if self._reqresp_socket is not None:
             rc = self._reqresp_socket.send_string("",zmq.Flag.SNDMORE);
             assert -1 != rc

        # store msg vor last value caching
        self._lvc[key] = {"size": msgSize, "msg": msgData}

        for peer_name, value in self._peer_map.items():
            if 1 == value[key]:
                self._reqresp_socket.send_string(peer_name, zmq.SNDMORE)
                self._reqresp_socket.send_string("",zmq.SNDMORE)

                send_more = True if 0 != msgSize and msgData is not None else False
                send_flags = zmq.SNDMORE if send_more else 0
                self._reqresp_socket.send_string(yc.KeyFmt % key, flags)

                if send_more:
                    self._reqresp_socket.send_string(msgData,0)

                rc = 1

                # if -1 == rc:
                #     if errno != zmq.EAGAIN:
                #         raise RuntimeError(f"Publisher::send: received error {zmq.strerror(errno)} from sending to peer {peer_name}")
                #     assert(errno == EAGAIN);
        return rc

    def _send_ident(self, key: int, msgSize: int , msgData: bytes, ident: str)-> int:
        rc = -1
        response_all = True if 0 == len(ident) else False

        if self._reqresp_socket is None:
            raise RuntimerError("_reqresp_socket_should not be none")

        if self._reqresp_socket is not None:
            for peer_name, value in self._peer_map.items():
                if not response_all and ident != peer_name:
                    continue

                self._reqresp_socket.send(peer_name, zmq.SNDMORE)
                self._reqresp_socket.send_string("",zmq.SNDMORE)
                send_more = True if 0 != msgSize and msgData is not None else False
                send_flags = zmq.SNDMORE if send_more else 0
                self._reqresp_socket.send_string(yc.KeyFmt % key, send_flags)

                if send_more:
                    self._reqresp_socket.send_string(msgData,0)

        return 1

    def receive(self) -> tuple[int,int,bytes,str]:
        bytes = 0
        more = -1
        key = None
        msgSize = 0
        msgData = None
        ident = None

        assert self._reqresp_socket is not None
        if self._reqresp_socket is not None:
            rc = self._reqresp_socket.poll(0, zmq.PollEvent.POLLIN)

            if rc == zmq.PollEvent.POLLIN:
                address = self._reqresp_socket.recv(zmq.NOBLOCK)
                if 0 < len(address):
                    ident = address
                    # receive empty block
                    empty = self._reqresp_socket.recv(zmq.NOBLOCK)
                    assert 0 == len(empty)
                    # receive key
                    key_str = self._reqresp_socket.recv(zmq.NOBLOCK)
                    if 0 < len(key_str):
                        key = int(key_str)
                        if 0 <= key:
                            logging.info(f"Publisher::receive peer {ident} with key {key}")
                            more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                            if more:
                                msg = self._reqresp_socket.recv(zmq.NOBLOCK)
                                more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                                if more:
                                    raise RuntimeError(f"Publisher:receive {ident} with key {key} unexpected end")
                        elif key == yc.KeySync:
                            logging.info(f"Publisher::receive peer {ident} KeySync")
                            more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                            if more:
                                raise RuntimeError("Publisher:receive KeySync unexpected end")

                            logging.info(f"Publisher::receive peer {ident} KeySync 1")
                            self._peer_map[ident] = {"key": -1, "value": 0}

                            self._reqresp_socket.send_multipart([address,b"",(yc.KeyFmt % key).encode("ascii"),yc.SynAck.encode("ascii")])

                            logging.info(f"Publisher::receive peer {ident} KeySync sended Response")

                        elif key == yc.KeySetNotification:
                            logging.info(f"Publisher::receive peer {ident} KeySetNotification")
                            notification_key_str = self._reqresp_socket.recv(zmq.NOBLOCK)
                            if 0 < len(key_str):
                                notification_key = int(notification_key_str)
                                logging.info(f"Publisher::receive peer {ident} KeySetNotification {notification_key}")
                                self._peer_map[ident] = {"key": notification_key, "value": 1}
                                more = self._reqresp_socket.getsockopt(zmq.RCVMORE)
                                if more:
                                    raise RuntimeError("Publisher:receive KeySetNotification unexpected end")

                                #
                                # send values from LVC Cache
                                if notification_key in self._lvc_map:
                                    lvc = self._lvc_map[notification_key]
                                    if 0 < len(lvc["msg"]):
                                        logging.info(f"Publisher::receive sending LVC cache values on {notification_key}")
                                        self.send(notification_key, lvc["msgSize"], lvc["msg"]);


                        # } else if (YaComponent::KeyClearNotification == key) {
                        #     char cNotKey[YaComponent::KeySize + 1];
                        #     memset(cNotKey, 0, YaComponent::KeySize + 1);
                        #     iBytes = zmq_recv(mpReqRespSocket, cNotKey, YaComponent::KeySize, ZMQ_NOBLOCK);
                        #     assert((0 < iBytes) && (iBytes == YaComponent::KeySize));
                        #     if (0 < iBytes) {
                        #         int notKey = -1;
                        #         sscanf(cNotKey, YaComponent::KeyFmt, &notKey);
                        #         mPeerMap[ident][notKey] = 0;
                        #         rc = zmq_getsockopt(mpReqRespSocket, zmq.RCVMORE, &more, &moreSize);
                        #         if (-1 < rc && more) {
                        #             qFatal(
                        #                 "YaPUBImpl::receive KeyClearNotification unexpected end");
                        # } else if (YaComponent::KeyEnd == key) {
                        #     qDebug() << "peer " << ident.c_str() << " closes";
                        #     mPeerMap.erase(ident);
                        #     rc = zmq_getsockopt(mpReqRespSocket, zmq.RCVMORE, &more, &moreSize);
                        #     if (-1 < rc && more) {
                        #         qFatal("YaPUBImpl::receive KeyEnd unexpected end");

                        # } else {
                        #     qFatal("%s",
                        #            qPrintable(QString("unknown key %1 from iBytes %2 to '%3'")
                        #                           .arg(key)
                        #                           .arg(iBytes)
                        #                           .arg(mcKeyReq)));

        return (key, msgSize, msgData, ident)


    def close(self):
        self._send_ident(yc.KeyEnd,0,0,"")
        self._reqresp_socket.close()
        self._reqresp_socket = None
