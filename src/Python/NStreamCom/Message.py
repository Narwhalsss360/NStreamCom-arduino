import math
from Packet import Packet

class ObjectNotPacket(Exception):
    def __init__(self, message):
        super().__init__(message)

class EmptyList(Exception):
    def __init__(self, message):
        super().__init__(message)

class PacketIDMismatch(Exception):
    def __init__(self, message):
        super().__init__(message)

class PacketMessageSizeMismatch(Exception):
    def __init__(self, message):
        super().__init__(message)

def verify_packet_list(packets : list) -> None:
    if len(packets) == 0:
        raise EmptyList("packets list was empty")

    for packet in packets:
        if not isinstance(packet, Packet):
            raise ObjectNotPacket(f"{packet} was not of type Packet")

    id = packets[0].id
    message_size = packets[0].message_size
    bytes_sum = 0
    
    for packet in packets:
        if id != packet.id:
            raise PacketIDMismatch("IDs of packets did not match")
        
        if message_size != packet.message_size:
            raise PacketMessageSizeMismatch("Message size of packets did not match")
        
        bytes_sum += len(packet.data)
      
    if bytes_sum != message_size:
        raise PacketMessageSizeMismatch("Sum of data from packets did not match message size")
    

class Message:
    def __init__(self, packets : list = None, id : int = None, data : list = None) -> None:
        if isinstance(id, int) and isinstance(data, list):
            self.id = id
            self.data = data
        elif isinstance(packets, list):
            verify_packet_list(packets)
            self.id = packets[0].id
            self.data = []
            for packet in packets:
                self.data += packet.data
        else:
            self.id = None
            self.data = None

    def get_packets(self, packet_size : int) -> list:
        if self.id is None or self.data is None:
            return None
            
        if packet_size >= len(self.data):
            return [ Packet(id = self.id, message_size = len(self.data), data = self.data) ]
        
        packet_count = math.ceil(len(self.data) / packet_size)
        
        packets = []
        for i_packet in range(packet_count):
            packets.append(Packet(id = self.id, message_size = len(self.data), data = self.data[i_packet * packet_size]))
        return packets