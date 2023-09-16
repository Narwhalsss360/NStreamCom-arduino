import math
from Packet import Packet

class ObjectNotPacket(Exception):
    def __init__(self, message):
        super.__init__(ObjectNotPacket, self, message)

class EmptyList(Exception):
    def __init__(self, message):
        super.__init__(EmptyList, self, message)

class PacketIDMismatch(Exception):
    def __init__(self, message):
        super.__init__(PacketIDMismatch, self, message)

class PacketMessageSizeMismatch(Exception):
    def __init__(self, message):
        super.__init__(PacketMessageSizeMismatch, self, message)

def verify_packet_list(packets : list) -> None:
    if len(packets) == 0:
        raise EmptyList("packets list was empty")

    for packet in packets:
        if packet is not Packet:
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
    def __init__(self, id : int, data : list) -> None:
        self.id = id
        self.data = data
        
    def __init__(self, packets : list) -> None:
        verify_packet_list(packets)
        self.id = packets[0].id
        self.data = []
        
        for packet in packets:
            self.data += packets.data

    def get_packets(self, packet_size : int) -> list:
        if packet_size >= len(self.data):
            return [ Packet(self.id, len(self.data), self.data) ]
        
        packet_count = math.ceil(len(self.data) / packet_size)
        
        packets = []
        for i_packet in range(packet_count):
            packets += Packet(self.id, len(self.data) - (i_packet * packet_size) if i_packet == packet_count - 1 else packet_size, self.data[i_packet * packet_size])
            
        return packets