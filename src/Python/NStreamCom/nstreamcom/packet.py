'''Packet class'''

from dataclasses import dataclass, field
from typing import Self, Sequence, Optional
from .constants import NSTREAMCOM_PROTOCOL_SIZE
from .flatten import flatten

def validate_bytes(stream_bytes: Sequence) -> bool:
    '''Check whether a sequence of bytes can produce a valid packet'''

    if len(stream_bytes) < NSTREAMCOM_PROTOCOL_SIZE:
        return False
    return len(stream_bytes) - NSTREAMCOM_PROTOCOL_SIZE == int.from_bytes(stream_bytes[4:6], 'little')


@dataclass(slots=True)
class Packet:
    '''Packet class for creating message and sending data'''

    id: int = field(default=-1)
    data: bytearray = field(default_factory=lambda: bytearray())
    message_size: int = field(default=-1)

    @staticmethod
    def from_bytes(stream_bytes: Sequence) -> Self:
        '''Create a packet from bytes'''

        return Packet(
                int.from_bytes(stream_bytes[0:2], 'little'),
                stream_bytes[NSTREAMCOM_PROTOCOL_SIZE:],
                int.from_bytes(stream_bytes[2:4], 'little')) if validate_bytes(stream_bytes) else Packet()

    def to_bytes(self) -> Optional[bytearray]:
        '''Get stream bytes to send to receiver for re-interpretation'''

        if not self:
            return None

        return bytearray(flatten([
            int.to_bytes(self.id, 2, 'little'),
            int.to_bytes(self.message_size, 2, 'little'),
            int.to_bytes(len(self.data), 2, 'little'),
            self.data
            ]))

    def valid(self) -> bool:
        '''Check whether this packet is valid and can be used'''

        return self.id != -1 and self.message_size != -1

    def __bool__(self) -> bool:
        '''bool operator for valid'''

        return self.valid()


def validate_packet_list(packets: Sequence[Packet]) -> bool:
    '''Checks whether the packet list can create a message'''

    if len(packets) == 0:
        return False

    message_id = packets[0].id
    message_size = packets[0].message_size

    for packet in packets:
        if not packet:
            return False
        if packet.id != message_id:
            return False
        if packet.message_size != message_size:
            return False

    return True
