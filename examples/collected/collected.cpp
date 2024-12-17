#include <nstreamcom.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

//Use a buffered decoder that uses a vector<uint8_t> as it's buffer
using vector_decoder = nstreamcom::buffered_decoder<
    std::vector<uint8_t>::iterator
>;

//Define a collector that uses a vector_decoder
using vector_collector = nstreamcom::collector<vector_decoder>;

bool try_resize_buffer(std::vector<uint8_t>& buffer, vector_collector& collector, uint8_t collecting) {
    using nstreamcom::collector_states;

    if (collector.state() != collector_states::BUFFER_FULL) {
        return true;
    }

    //Resize, and notify collector
    int index = collector.decoder().position() - buffer.begin();
    int previous_index = index == 0 ? 0 : index - 1;
    buffer.resize(buffer.size() + 1);

    collector.decoder().use(
        buffer.begin(),
        buffer.end(),
        buffer.begin() + index,
        buffer.begin() + previous_index
    );
    collector.expanded(); //Notify that buffer is no longer full

    collector_states recollect_state = collector.collect(collecting);
    if (recollect_state == collector_states::BUFFER_FULL) {
        return false;
    }

    return true;
}

int main() {
    using std::cin;
    using std::cout;
    using std::endl;
    using std::getline;
    using std::string;
    using std::vector;

    //Get the size of encoded data plus encoded size
    using nstreamcom::as_collected_size;

    //Encodes size, then data into a buffer of the collected size
    using nstreamcom::encode_with_size;

    //Used to determine the current state of the collector
    using nstreamcom::collector_states;

    //Buffer to use
    vector<uint8_t> buffer;

    //See `using` in global scope above
    vector_collector collector = vector_collector(buffer.begin(), buffer.end());

    cout << "Enter text when prompted (>) to test encoding/decoding using collector.\n";
    cout << "The program will take text input, encode it, use collector, and print the decoded string.\n";
    cout << "Type '.exit' to exit.\n";

    while (true) {
        cout << '>';
        string line;
        getline(cin, line);

        if (line == ".exit") {
            break;
        }

        vector<uint8_t> encoded_with_size = vector<uint8_t>(as_collected_size(line.size()));

        //Requires `enocded` buffer to be of size as_collected_size(*data size*)
        encode_with_size(
            (uint8_t*)&*line.begin(),
            (uint8_t*)&*line.end(),
            line.size(),
            encoded_with_size.begin(),
            encoded_with_size.end()
        );

        for (uint8_t byte : encoded_with_size) {
            //Collect the next byte
            collector_states state = collector.collect(byte);

            if (state == collector_states::BUFFER_FULL) {
                //Tries to resize buffer, see function definition
                if (!try_resize_buffer(buffer, collector, byte)) {
                    cout << "Error, collect after buffer resize failed..." << endl;;
                    return 1;
                }
            } else if (state == collector_states::MISSING_SIZE) {
                cout << "Error, missing size...\n";
                collector.reset();
                break;
            } else if (state == collector_states::MISSING_DATA) {
                cout << "Error, missing data...\n";
                collector.reset();
                break;
            }
        }

        //When state is collected, then all data is received and ready to be read from the buffer
        if (collector.state() != collector_states::COLLECTED) {
            cout << "Collector did not collect, state code:" << (int)collector.state() << '\n';
            collector.reset();
            continue;
        }

        string decoded;

        //buffer.size() may be bigger than actual data now, collector has size for the data ready now.
        decoded.resize(collector.next_size()); //next_size() is the size of the data

        //Copy from the buffer the collector uses. Only valid when state is COLLECTED
        copy(
            buffer.begin(),
            buffer.begin() + (int)decoded.size(),
            (uint8_t*)&*decoded.begin()
        );

        cout << decoded << '\n';

        //collector automatically resets on next collect if current state is COLLECTED.
    }

    return 0;
}
