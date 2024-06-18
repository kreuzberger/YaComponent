class Publisher():

    def __init__(self, ctx):
        pass

    def setConnectionPara(self, address: str, hwm: int = 0):
        pass

    def send(self, key: int, msg)-> int: #const ::google::protobuf::MessageLite
        return -1

    def response(self, key: int, msg, ident: str)-> int: # const ::google::protobuf::MessageLite
        return -1

    def send(self, key: int, msgSize: int, msgData: bytes)-> int:
        return -1

    def send(self, key: int, msgSize: int , msgData: bytes, ident: str)-> int:
        return -1

    def receive(self, key: int, size: int, data , ident: str) -> int:
        return -1

    def close(self):
        pass
