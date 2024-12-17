#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <nstreamcom.h>

int main() {
    using std::cout;
    using std::cin;
    using std::string;
    using std::getline;
    using std::vector;
    using std::copy;

    using nstreamcom::nsize_int;
    using nstreamcom::as_transmission_size;
    using nstreamcom::encode;
    using nstreamcom::buffered_decoder;


    //Specify what iterator is being used for the decoder
    using vector_decoder = buffered_decoder<
        vector<uint8_t>::iterator
    >;

    vector<uint8_t> buffer;
    vector_decoder decoder = vector_decoder(buffer.begin(), buffer.end()); //Create by supplying iterators

    while (true) {
        string line;
        cout << '>';
        getline(cin, line);

        if (line.size() == 0) {
            continue;
        }

        if (line == ".exit") {
            break;
        }

        /*
            Casts to uint8_t* is dont *just* for compiler warning of mismatch of
            signed/unsigned char, copy works as long bits copy over.
        */

        vector<uint8_t> encoded = vector<uint8_t>(as_transmission_size(line.size()));
        encode((uint8_t*)&*line.begin(), (uint8_t*)&*line.end(), encoded.begin(), encoded.end());

        for (nsize_int i = 0; i < encoded.size(); i++) {
            //Returns false if buffer is full.
            if (!decoder.next(encoded[i], i == encoded.size() - 1)) {
                //These indices are required because after resizing, iterators would be invalidated.
                int buffer_index = decoder.position() - buffer.begin();

                //Must be index - 1, but not -1
                int previous_index = buffer_index == 0 ? 0 : buffer_index - 1;

                buffer.resize(buffer.size() + 1);

                //After resize, iterators may be invalidated
                decoder.use(
                    buffer.begin(),
                    buffer.end(),
                    buffer.begin() + buffer_index,
                    buffer.begin() + previous_index
                );

                i--; //Redo this byte after resize.
            }
        }

        string decoded;
        decoded.resize(line.size()); // Size information must be known (either encoded or data)
        copy(buffer.begin(), buffer.end(), (uint8_t*)&*decoded.begin());

        cout << decoded << '\n';
        decoder.reset();
    }
}
