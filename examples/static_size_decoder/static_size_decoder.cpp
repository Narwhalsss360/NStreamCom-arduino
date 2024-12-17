#include <iostream>
#include <string>
#include <algorithm>
#include <nstreamcom.h>

using nstreamcom::nsize_int;

constexpr nsize_int buffer_size = 1024;

int main() {
    using std::cout;
    using std::cin;
    using std::string;
    using std::getline;
    using std::copy;

    //Used to determine size of buffer for encoded data, from size of any data.
    using nstreamcom::as_transmission_size;

    //Encodes data, in this example, the overload that accepts any type, and properly sized encoded buffer is used
    using nstreamcom::encode;

    //Static sized decoder that is in stack memory.
    using nstreamcom::stack_buffered_decoder;

    char buffer[buffer_size];

    //Decodes bytes as they come in, useful for when data has to be buffered in.
    stack_buffered_decoder<buffer_size> decoder;

    cout << "Enter text when prompted (>) to test encoding/buffered decoding with a constant sized buffer.\n";
    cout << "The program will take text input, encode it, then decode an print the decoded string.\n";
    cout << "Max line length:" << buffer_size << '\n';
    cout << "Type '.exit' to exit.\n";

    while (true) {
        string line;
        cout << '>';
        getline(cin, line);

        if (line == ".exit") {
            break;
        }

        if (buffer_size < line.size() + 1) {
            cout << "Error, line too long for buffer size " << buffer_size << '\n';
            continue;
        }

        //+ 1 to include \0
        copy(line.c_str(), line.c_str() + line.size() + 1, buffer);

        uint8_t encoded[as_transmission_size(buffer_size)];
        encode(buffer, encoded); //`sizeof(encoded)` must exactly equal `as_transmisson_size(sizeof(buffer))`

        for (nsize_int i = 0; i < sizeof(encoded); i++) {
            //Returns false if buffer is full.
            if (!decoder.next(encoded[i], i == sizeof(encoded) - 1)) {
                cout << "stack decoder buffer full, out of memory.";
                decoder.reset();
            }
        }

        const char (&decoded)[buffer_size] = (const char(&)[buffer_size])decoder.array();
        cout << decoded << '\n';

        //Sets decoder ready for new data.
        decoder.reset();
    }

    return 0;
}
