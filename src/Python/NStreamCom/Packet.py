STREAM_PROTOCOL_SIZE = 6

class PacketSizeDataMismatch(Exception):
    def __init__(self, message : str):
        super().__init__(message)

def verify_bytes(data : list):
    if len(data) < STREAM_PROTOCOL_SIZE:
        raise PacketSizeDataMismatch('Length of list was less than minimum')
    
    packet_size = int.from_bytes(data[4:6], 'little')

    if len(data) - STREAM_PROTOCOL_SIZE != packet_size:
        raise PacketSizeDataMismatch("Length of list did not match its size with protocol")

class Packet:
    def __init__(self, id : int = None, message_size : int = None, data : list = None):
        if id is not None and message_size is not None:
            self.id = id
            self.message_size = message_size
            self.data = data if isinstance(data, list) else [data]
        elif isinstance(data, list):
            verify_bytes(data = data)
            self.id = int.from_bytes(data[0:2], 'little')
            self.message_size = int.from_bytes(data[2:4], 'little')
            self.data = data[6:]
        else:
            self.id = None;
            self.message_size = None;
            self.data = None;

    def get_stream_bytes(self) -> list:
        if self.id is None or self.message_size is None:
            return None
        stream_bytes = []
        stream_bytes +=  int.to_bytes(self.id, 2, 'little')
        stream_bytes += int.to_bytes(self.message_size, 2, 'little')
        stream_bytes += int.to_bytes(len(self.data), 2, 'little')
        if self.data is not None:
            stream_bytes += self.data
        return stream_bytes