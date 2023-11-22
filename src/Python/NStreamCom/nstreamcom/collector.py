'''PacketCollector class'''

from typing import Callable, Optional, Sequence
from .packet import Packet

class PacketCollector:
    '''PacketCollector for collecting packets and interpreting when ready'''

    def __init__(self, packets_ready: Optional[Callable] = None) -> None:
        self._packets = []
        self._bytes_collected = 0
        self.packets_ready = packets_ready

    def discard(self) -> None:
        '''Discard received packets'''

        self._packets.clear()
        self._bytes_collected = 0

    def collect(self, stream_bytes: Sequence) -> bool:
        '''Collect bytes to iterpret packet and collect to wait for whole message'''

        recycle = False
        collected = Packet.from_bytes(stream_bytes)
        if not collected:
            self.discard()
            return False
        self._bytes_collected += len(collected.data)

        if len(self._packets) == 0:
            self._packets.append(collected)
            if collected.message_size != len(collected.data):
                return True
        else:
            if collected.id == self._packets[-1].id:
                self._packets.append(collected)
                if collected.message_size != self._bytes_collected:
                    return True
            else:
                if self._bytes_collected != self._packets[-1].message_size:
                    self.discard()
                    return False
                recycle = True

        if callable(self.packets_ready):
            self.packets_ready(self._packets)
        self.discard()
        if recycle:
            self.collect(stream_bytes)
        return True
