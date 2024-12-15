#include <nstreamcom.h>
#include <iostream>

struct stack_str {
    static constexpr const size_t SIZE = 64;

    char str[SIZE];
    size_t length = 0;

    stack_str(const char* init_str = "") {
        set(init_str);
    }

    void set(const char* as) {
        const char* src = as;
        char* dst = str;
        size_t i = 0;
        while (*src != '\0' && i < SIZE) {
            *dst = *src;
            ++src;
            ++dst;
            ++i;
        }
        length = i - 1;
    }
};

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

    const stack_str custom_type("Lorem ipsum dolor sit amet, consectetur adipiscing nunc");
    uint8_t encoded_custom_type[as_transmission_size(sizeof(custom_type))];
    encode(custom_type, encoded_custom_type, encoded_custom_type + sizeof(encoded_custom_type));

    stack_str decoded_custom_type;
    decode(encoded_custom_type, encoded_custom_type + sizeof(encoded_custom_type), decoded_custom_type);

    if (custom_type.length != decoded_custom_type.length) {
        cout << "Error, encode/decode, line:" << __LINE__ << endl;
        return 1;
    }

    for (size_t i = 0; i < custom_type.length; i++) {
        if (custom_type.str[i] != decoded_custom_type.str[i]) {
            cout << "Error, encode/decode, line:" << __LINE__ << endl;
            return 1;
        }
    }

    uint8_t encoded_custom_type_2[as_transmission_size(sizeof(custom_type))];
    encode(custom_type, encoded_custom_type_2);

    if (sizeof(encoded_custom_type) != sizeof(encoded_custom_type_2)) {
        cout << "Error, differing sizes for same data, line:" << __LINE__ << endl;
        return 1;
    }

    for (size_t i = 0; i < sizeof(encoded_custom_type); i++) {
        if (encoded_custom_type[i] != encoded_custom_type_2[i]) {
            cout << "Error, differing encoded data for same data, line:" << __LINE__ << endl;
            return 1;
        }
    }

    stack_str decoded_custom_type_2;
    decode(encoded_custom_type_2, decoded_custom_type_2);

    if (custom_type.length != decoded_custom_type_2.length) {
        cout << "Error, encode/decode, line:" << __LINE__ << endl;
        return 1;
    }

    for (size_t i = 0; i < custom_type.length; i++) {
        if (custom_type.str[i] != decoded_custom_type_2.str[i]) {
            cout << "Error, encode/decode, line:" << __LINE__ << endl;
            return 1;
        }
    }



    cout << "Success!" << endl;
    return 0;
}
