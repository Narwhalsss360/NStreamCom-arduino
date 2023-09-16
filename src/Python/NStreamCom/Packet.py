STREAM_PROTOCOL_SIZE = 6

class PacketSizeDataMismatch(Exception):
    def __init__(self, message : str):
        super.__init__(PacketSizeDataMismatch, self, message)

def verify_bytes(data : list):
    if len(data) < STREAM_PROTOCOL_SIZE:
        raise PacketSizeDataMismatch('Length of list was less than minimum')
    
    message_size = int.from_bytes(data[4:7], 'little')

    if len(data) - STREAM_PROTOCOL_SIZE != message_size:
        raise PacketSizeDataMismatch("Length of list did not match its size with protocol")
    
    return message_size

class Packet:
    def __init__(self, id : int = 0, message_size : int = 0, data = None):
        self.id = id
        self.message_size = message_size
        self.data = data

    def __init__(self, data):
        self.message_size = verify_bytes(data)
        self.id = int.from_bytes(data[0:2], 'little')
        self.data = data[STREAM_PROTOCOL_SIZE:]

    def get_stream_bytes(self) -> list:
        stream_bytes = []
        stream_bytes +=  int.to_bytes(self.id, 2, 'little')
        stream_bytes += int.to_bytes(self.message_size, 2, 'little')
        stream_bytes += int.to_bytes(len(self.data), 'little')
        if self.data is not None:
            stream_bytes += self.data
        return stream_bytes