'''Message class'''

from math import ceil
from dataclasses import dataclass, field
from typing import Self, Sequence
from .flatten import flatten
from .packet import Packet, validate_packet_list

@dataclass(slots=True)
class Message:
    '''Message class for sending/receiving messages using NStreamCom Protocol'''

    id: int = field(default=-1)
    data: bytearray = field(default_factory=lambda: bytearray())

    @staticmethod
    def from_packets(packets: Sequence[Packet]) -> Self:
        '''Interpret message from packets'''

        if not validate_packet_list(packets):
            return Message()
        return Message(packets[0].id, bytearray(flatten([packet.data for packet in packets])))

    def valid(self) -> bool:
        '''Check whether message is valid and can be used'''

        return self.id != -1

    def __bool__(self) -> bool:
        return self.valid()

    def to_packets(self, packet_size: int) -> list[Packet]:
        '''Turn this message into packets according to `packet_size`'''

        if not self:
            return []

        if packet_size < 1 or packet_size >= len(self.data):
            return [Packet(self.id, self.data, len(self.data))]

        packet_count = int(ceil(len(self.data) / packet_size))
        packets = []
        for i in range(packet_count):
            size = len(self.data) - (i * packet_size) if i == packet_count - 1 else packet_size
            start = len(self.data) - size if i == packet_count - 1 else i * packet_size
            packets.append(Packet(self.id, self.data[start:start + size], len(self.data)))
        return packets
