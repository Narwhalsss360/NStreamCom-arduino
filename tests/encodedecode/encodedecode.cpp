#include <nstreamcom.h>
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    using nstreamcom::as_transmission_size;
    using nstreamcom::as_data_size;
    using nstreamcom::encode;
    using nstreamcom::decode;

    const uint8_t str[] = "Lorem ipsum dolor sit amet, consectetur adipiscing nunc";

    uint8_t encoded_str[as_transmission_size(sizeof(str))];
    encode<const uint8_t*, uint8_t*>(str, str + sizeof(str), encoded_str, encoded_str + sizeof(encoded_str));

    uint8_t decoded_str[as_data_size(sizeof(encoded_str))];
    decode<uint8_t*, uint8_t*>(
        encoded_str,
        encoded_str + sizeof(encoded_str),
        decoded_str,
        decoded_str + sizeof(decoded_str)
    );

    if (sizeof(decoded_str) != sizeof(str)) {
        cout << "Error, size difference, line:" << __LINE__ << endl;
        return 1;
    }

    for (size_t i = 0; i < sizeof(str); i++) {
        if (str[i] != decoded_str[i]) {
            cout << "Error, data mismatch at index " << i << ", line:" << __LINE__ << endl;
            return 1;
        }
    }

    cout << "Success!" << endl;
    return 0;
}
