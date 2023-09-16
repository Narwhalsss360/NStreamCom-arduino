from Message import Message
from Packet import Packet, PacketSizeDataMismatch

class PacketMessageSizeMismatch(Exception):
    def __init__(self, message = None):
        super.__init__(PacketSizeDataMismatch, self, message)

class PacketCollector:
    def __init__(self) -> None:
        self.packets_collected = []
        self.bytes_collected = 0
        self.recycle_stream = False
        self.packet_ready_callback = None
        
    def discard(self) -> None:
        self.packets_collected.clear()
        self.bytes_collected = 0
        
    def collect(self, data) -> None:
        self.recycle_stream = False
        collected = None
        
        try:
            collected = Packet(data = data)
        except PacketSizeDataMismatch:
            self.discard()
            raise
        
        self.bytes_collected += len(collected.data)
        
        if len(self.packets_collected) == 0:
            self.packets_collected.append(collected)
            return
        else:
            if collected.id == self.packets_collected[-1].id:
                self.packets_collected.append(collected)
                if self.bytes_collected != collected.message_size:
                    return
            else:
                if self.bytes_collected != self.packets_collected[-1].message_size:
                    self.discard()
                    raise PacketMessageSizeMismatch("Did not receive all packets")
                self.recycle_stream = True

        if callable(self.packet_ready_callback):
            self.packet_ready_callback(self.packets_collected)
        self.discard()
        if (self.recycle_stream):
            self.collect(data)
          