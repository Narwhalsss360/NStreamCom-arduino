using NStreamCom;
using System.IO;
using System.IO.Ports;
using System.Text;

internal class Program
{
    const int PACKET_SIZE = 16;

    const int INTER_PACKET_WAIT_TIME_MS = 2;

    const int BAUDRATE = 115200;

    const int TOGGLE_LED_ID = 0xFFFE;

    const int LINE_COUNT_ID = 0xFFFF;

    private static SerialPort s_port = new() { BaudRate = BAUDRATE };

    private static ushort s_lineCount = 0xFFFF; //Line count of display, default is 0xFFFF

    private static PacketCollector s_collector = new();

    private static void ConnectedLoop()
    {
        while (true)
        {
            string entry = Console.ReadLine() ?? "";
            if (entry.ToLower() == "exit") break;

            if (entry.ToLower() == "line-count")
            {
                Console.WriteLine($"{s_lineCount} lines");
                continue;
            }

            if (entry.ToLower() == "led")
            {
                new Message(TOGGLE_LED_ID, Array.Empty<byte>()).WriteTo(PACKET_SIZE, s_port.BaseStream);
                continue;
            }

            int colonIndex = entry.IndexOf(':');
            if (colonIndex < 0)
            {
                Console.WriteLine("Bad syntax, use exit to exit. Use line-count to get line count, or led to toggle led. format: entry no.:string. ex. 0:Line 1");
                continue;
            }

            if (!ushort.TryParse(entry[0..colonIndex], out ushort lineNumber))
            {
                Console.WriteLine("Error! Line number was not e number.");
                continue;
            }

            if (lineNumber >= s_lineCount)
            {
                Console.WriteLine($"Error! over line-count, max={s_lineCount - 1}");
                continue;
            }

            entry = entry[(colonIndex + 1)..];

            //Writing a message:
            //Creat the message
            Message printMessage = new Message(lineNumber, Encoding.UTF8.GetBytes(entry));

            //Get it into packets
            Packet[] packets = printMessage.GetPackets(PACKET_SIZE);

            //Get the bytes to send for each packet
            byte[][] packetsBytes = packets.GetPacketsBytes();

            //Wrapper to send all bytes through a stream with a TimeSpan between each packet
            packetsBytes.WriteAllPacketsTo(s_port.BaseStream, TimeSpan.FromMilliseconds(INTER_PACKET_WAIT_TIME_MS));
        }
    }

    private static void Main(string[] args)
    {
        Console.Write("Select a port:");
        foreach (string portName in SerialPort.GetPortNames())
            Console.Write($"{portName} ");
        Console.WriteLine();
        s_port.PortName = Console.ReadLine();
        s_port.DataReceived += SerialDataReceived;
        s_collector.PacketsReady += PacketsReady;
        try
        {
            s_port.Open();
        }
        catch (Exception e)
        {
            Console.WriteLine($"An error occured trying to open the port: {e}: {e.Message}. {e.StackTrace}");
            return;
        }
        Console.WriteLine("Successfully opened port.");
        s_port.DiscardInBuffer();
        s_port.DiscardOutBuffer();
        Thread.Sleep(1);

        ConnectedLoop();
        s_port.Close();
    }

    private static void PacketsReady(object sender, PacketsReadyEventArgs args)
    {
        Message message = new Message(args.Packets);
        if (message.ID == LINE_COUNT_ID)
        {
            if (message.Data.Length == 0)
                return;

            s_lineCount = message.Data[0];
            Console.WriteLine($"Received line count: {s_lineCount}");
        }
    }

    private static void SerialDataReceived(object? sender, SerialDataReceivedEventArgs args)
    {
        if (args.EventType == SerialData.Eof)
            return;
        if (sender is not SerialPort port)
            return;

        try
        {
            byte[] bytes = new byte[port.BytesToRead];
            port.Read(bytes, 0, bytes.Length);
            s_collector.Collect(bytes);
        }
        catch (Exception e)
        {
            Console.WriteLine($"An error occured trying to read from the port: {e}: {e.Message}. {e.StackTrace}");
        }
    }
}