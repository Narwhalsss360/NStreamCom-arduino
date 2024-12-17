#include <nstreamcom.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    using std::cout;
    using std::cin;
    using std::string;
    using std::getline;
    using std::vector;

    //Used to determine size of buffer for encoded data, from size of any data.
    using nstreamcom::as_transmission_size;

    //Used to determine size of buffer for data, from size of encoded data.
    using nstreamcom::as_data_size;

    //Encodes data, in this example, the overload that uses iterators is used.
    using nstreamcom::encode;

    //Decodes data, in this example, the overload that uses iterators is used.
    using nstreamcom::decode;

    //Used to cast to nsize_int type, for getting rid of warning of cast to smaller size.
    using nstreamcom::nsize_int;

    cout << "Enter text when prompted (>) to test encoding/decoding with a dynamic sized buffer.\n";
    cout << "The program will take text input, encode it, then decode an print the decoded string.\n";
    cout << "Type '.exit' to exit.\n";

    while (true) {
        string line;
        cout << '>';
        getline(cin, line);

        if (line == ".exit") {
            break;
        }

        /*
            Casts to uint8_t* is dont *just* for compiler warning of mismatch of
            signed/unsigned char, copy works as long bits copy over.
        */

        vector<uint8_t> encoded;
        encoded.resize(as_transmission_size((nsize_int)line.size()));
        encode((uint8_t*)&*line.begin(), (uint8_t*)&*line.end(), encoded.begin(), encoded.end());

        string decoded;
        decoded.resize(as_data_size((nsize_int)encoded.size()));
        decode(encoded.begin(), encoded.end(), (uint8_t*)&*decoded.begin(), (uint8_t*)&*decoded.end());

        cout << decoded << '\n';
    }

    return 0;
}
