#include <iostream>
#include <string>
#include <algorithm>
#include <nstreamcom.h>

constexpr nstreamcom::nsize_int buffer_size = 1024;

int main() {
    using std::cout;
    using std::cin;
    using std::string;
    using std::getline;
    using std::copy;

    //Used to determine size of buffer for encoded data, from size of any data.
    using nstreamcom::as_transmission_size;

    //Used to determine size of buffer for data, from size of encoded data.
    using nstreamcom::as_data_size;

    //Encodes data, in this example, the overload that accepts any type, and properly sized encoded buffer is used
    using nstreamcom::encode;

    //Decodes data, in this example, the overload that accepts any type, and properly sized encoded buffer is used
    using nstreamcom::decode;

    char buffer[buffer_size];

    cout << "Enter text when prompted (>) to test encoding/decoding with a constant sized buffer.\n";
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

        char decoded[as_data_size(sizeof(encoded))];
        decode(encoded, decoded); //`sizeof(decoded)` must exactly equal `as_data_size(sizeof(encoded))`

        cout << decoded << '\n';
    }

    return 0;
}
