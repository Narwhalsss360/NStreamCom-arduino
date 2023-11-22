from nstreamcom import Packet, Message, PacketCollector, flatten, validate_packet_list

PACKET_SIZE = 8
TEXT = '''Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc congue,
dui et tempor hendrerit, justo dolor varius diam, ut vulputate orci ex ut
leo. Quisque non mauris non elit pharetra consequat a vitae dolor. Mauris
luctus, ex et maximus sagittis, tellus ex aliquam nisi, non dictum nunc metus
id leo. Pellentesque mattis ornare tellus, sollicitudin pellentesque nisl
vulputate sit amet. Pellentesque habitant morbi tristique senectus et netus
et malesuada fames ac turpis egestas.'''

def serialize_deserialize_test():
    tx_messaage = Message(1, TEXT.encode())
    tx_packets = tx_messaage.to_packets(PACKET_SIZE)
    tx_packets_bytes = [packet.to_bytes() for packet in tx_packets]
    rx_packets = [Packet.from_bytes(stream_bytes) for stream_bytes in tx_packets_bytes]
    rx_message = Message.from_packets(rx_packets)
    if rx_message.data.decode() != TEXT:
        raise Exception('Serialize/Deserialize error')

def packet_collector_test():
    tx_messaage = Message(1, TEXT.encode())
    tx_packets = tx_messaage.to_packets(PACKET_SIZE)
    tx_packets_bytes = [packet.to_bytes() for packet in tx_packets]

    def packets_ready(packets: list[Packet]):
        if not validate_packet_list(packets):
            raise Exception('Protocol parse error')
        message = Message.from_packets(packets)
        if not message:
            raise Exception('Protocol parse error')
        if message.data.decode() != TEXT:
            raise Exception('Data parse error')
        print(f'Successfully collected: {message.data.decode()}')

    collector = PacketCollector(packets_ready)
    for packet_bytes in tx_packets_bytes:
        if not collector.collect(packet_bytes):
            raise Exception('Collection error')

def main():
    serialize_deserialize_test()
    packet_collector_test()

if __name__ == '__main__':
    main()
