from Message import Message
from Packet import Packet
from PacketCollector import PacketCollector

def serialize_deserialize_test():
    num = int(124)
    tx_num_bytes = int.to_bytes(num, 4, 'little')
    tx_message = Message(id = 1, data = list(tx_num_bytes))
    tx_packets = tx_message.get_packets(1)
    tx_streams_bytes = [packet.get_stream_bytes() for packet in tx_packets]
    collector = PacketCollector()
    
    def packets_collected_callback(packets_collected):
        rx_message = Message(packets_collected)
        if rx_message.id != tx_message.id:
            raise ""
        if rx_message.data != tx_message.data:
            raise ""

    collector.packet_ready_callback = packets_collected_callback

    for tx_stream_bytes in tx_streams_bytes:
        collector.collect(tx_stream_bytes)
    
    

def main():
    serialize_deserialize_test()

if __name__ == '__main__':
    main()