# NStreamCom
NStreamCom is a simple (de)serilization protocol for sending/receiving messages to a
device through a stream, or any other medium that uses bytes.

# Basic Understanding
This protocol uses a `Message` to either be interpreted by your program, or to be
partitioned into packets. Each message has an ID. Each packet contains at minimum
6 bytes of data described in [Buffer Template](https://github.com/Narwhalsss360/NStreamCom/blob/main/lib/BufferTemplate.csv).

