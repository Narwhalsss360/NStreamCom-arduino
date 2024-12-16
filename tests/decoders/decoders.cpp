#include <iostream>
#include <nstreamcom.h>
#include <vector>

int main() {
    using std::cout;
    using std::endl;
    using std::vector;
    using nstreamcom::as_transmission_size;
    using nstreamcom::encode;
    using nstreamcom::buffered_decoder;
    using nstreamcom::stack_buffered_decoder;

    const char str[] = "61b812d97657c7bc755582fac4712ab8735173fa";
    uint8_t encoded_str[as_transmission_size(sizeof(str))];
    encode(str, encoded_str);

    stack_buffered_decoder<as_transmission_size(sizeof(str))> decoder;

    for (size_t i = 0; i < sizeof(encoded_str); i++) {
        if (!decoder.next(encoded_str[i], i == sizeof(encoded_str) - 1)) {
            cout << "Error, buffer full" << endl;
            return 1;
        }
    }

    const char (&decoded_str)[sizeof(str)] = (const char(&)[sizeof(str)])decoder.array();

    for (size_t i = 0; i < sizeof(str); i++) {
        if (decoded_str[i] != str[i]) {
            cout << "Error, encode/decode" << endl;
            return 1;
        }
    }

    vector<uint8_t> buffer;
    auto heap_decoder = buffered_decoder<vector<uint8_t>::iterator>(buffer.begin(), buffer.end());
    for (int i = 0; i < sizeof(encoded_str); i++) {
        if (!heap_decoder.next(encoded_str[i], i == sizeof(encoded_str) - 1)) {
            int index = heap_decoder.position() - buffer.begin();
            buffer.resize(buffer.size() + 1);
            heap_decoder.use(
                buffer.begin(),
                buffer.end(),
                buffer.begin() + index,
                buffer.begin() + (index == 0 ? 0 : index - 1)
            );
            i--;
        }
    }

    for (size_t i = 0; i < sizeof(str); i++) {
        if (buffer[i] != str[i]) {
            cout << "Error, encode/decode" << endl;
            return 1;
        }
    }

    cout << "Success!" << endl;
    return 0;
}
