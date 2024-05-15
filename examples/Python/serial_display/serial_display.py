from sys import path
from time import sleep
from serial import Serial

path.append('../../../src/Python/NStreamCom')
path.append('src/Python/NStreamCom')

from nstreamcom import *

collector = PacketCollector()

PACKET_SIZE = 16
INTER_PACKET_WAIT_TIME_MS = 1
BAUDRATE = 115200
TOGGLE_LED_ID = 0xFFFE
LINE_COUNT_ID = 0xFFFF

line_count = 0xffff
with Serial(input('Enter port name:'), BAUDRATE, timeout=0.1) as port:
    def write_message(message: Message, packet_size: int):
        packets = message.to_packets(packet_size)
        packets_bytes = [packet.to_bytes() for packet in packets]
        for packet_bytes in packets_bytes:
            res = port.write(packet_bytes)

    def packets_ready(packets):
        global line_count
        message = Message.from_packets(packets)
        if message.id == LINE_COUNT_ID:
            if len(message.data) == 0:
                return
            line_count = message.data[0]

    collector.packets_ready = packets_ready

    while True:
        entry = input()

        if port.in_waiting > 0:
            if not collector.collect(port.read_all()):
                print('Collector error')

        if entry.lower() == 'exit':
            break
        if entry.lower() == 'line-count':
            print(f'{line_count=}')
            continue
        if entry.lower() == 'led':
            write_message(Message(id=TOGGLE_LED_ID), PACKET_SIZE)
            continue

        if ':' not in entry:
            print('Syntax->no.:str')
            continue

        try:
            line = int(entry[:entry.index(':')])
        except ValueError:
            continue

        write_message(Message(
                        id=line,
                        data=bytes(entry[entry.index(':') + 1:], 'utf-8')),
                    PACKET_SIZE)
