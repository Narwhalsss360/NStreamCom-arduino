#include <NStreamCom.h>

constexpr const unsigned long BAUDRATE = 115200;

//Each packet max size. Choose number that isnt too big, nor to small.
constexpr const uint16_t PACKET_SIZE = 16; 

/*
    Wait a little before sending each packet to be able to differentiate.
    You may try to use dtr, and rts
*/
constexpr const uint32_t INTER_PACKET_WAIT_TIME = 1;

//If the board receives a message with this id, it toggles its led.
constexpr const messageid_t TOGGLE_LED_ID = 0xFFFE;

/*
    If the board receives a message with its id, it echoes the line count of the
    board with the same LINE_COUNT_ID message id.
*/
constexpr const messageid_t LINE_COUNT_ID = 0xFFFF;

/*
    Timeout for readString()
*/
constexpr const unsigned long SERIAL_TIMEOUT_MS = 10;

PacketCollector collector;

constexpr const uint16_t LINE_COUNT = 4;
void displayLine(uint8_t line, String& s)
{
    /*Your code to display a string on line, for example an LCD*/
}

void sendLineCount()
{
    uint16_t lineCount = LINE_COUNT;
    Message message = Message(LINE_COUNT_ID, &lineCount, sizeof(lineCount));
    message.fastWrite(PACKET_SIZE, Serial, INTER_PACKET_WAIT_TIME);
}

void messageReady(const messageid_t id, void* data, uint16_t size)
{
    if (id == LINE_COUNT_ID)
    {
        sendLineCount();
        return;
    }
    else if (id == TOGGLE_LED_ID)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        return;
    }

    if (id >= LINE_COUNT)
        return;

    String s;
    if (!s.reserve(size))
        return;

    uint8_t* asBytes = (uint8_t*)data;

    for (int i = 0; i < size; i++)
        s.setCharAt(i, asBytes[i]);

    displayLine(id, s);
}

void serialEvent()
{
    String read = Serial.readString();
    if (!collector.collect((uint8_t*)read.c_str(), read.length()))
    { /*Collector error*/ }
}

void setup()
{
    Serial.begin(BAUDRATE);
    Serial.setTimeout(SERIAL_TIMEOUT_MS);
    pinMode(LED_BUILTIN, OUTPUT);
    collector.messageReady(messageReady);
    while (!Serial);
    Serial.flush();
    sendLineCount();
}

void loop()
{
//    if (Serial.available()) serialEvent(); //Uncomment for boards with no automatic serialEvent().
}
