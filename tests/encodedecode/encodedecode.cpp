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
    using nstreamcom::nsize;
    using nstreamcom::as_transmission_size;
    using nstreamcom::as_collected_size;
    using nstreamcom::as_data_size;
    using nstreamcom::encode;
    using nstreamcom::encode_with_size;
    using nstreamcom::decode;
    using nstreamcom::ENCODED_NSIZE_SIZE;

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

    uint8_t encoded_custom_type_with_size[as_collected_size(sizeof(custom_type))];
    encode_with_size(custom_type, encoded_custom_type_with_size);

    nsize decoded_size = nsize((nsize::nsize_int_bytes&)encoded_custom_type_with_size);
    decoded_size.decode();
    stack_str decoded_custom_type_with_size;

    if (decoded_size != sizeof(custom_type)) {
        cout << "Error, encoded size mismatch, line:" << __LINE__ << endl;
        return 1;
    }

    decode(
        encoded_custom_type_with_size + ENCODED_NSIZE_SIZE,
        encoded_custom_type_with_size + sizeof(encoded_custom_type_with_size),
        decoded_custom_type_with_size
    );

    if (custom_type.length != decoded_custom_type_with_size.length) {
        cout << "Error, encode/decode, line:" << __LINE__ << endl;
        return 1;
    }

    for (size_t i = 0; i < custom_type.length; i++) {
        if (custom_type.str[i] != decoded_custom_type_with_size.str[i]) {
            cout << "Error, encode/decode, line:" << __LINE__ << endl;
            return 1;
        }
    }

    const uint8_t byte_array[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint8_t encoded_byte_array[as_transmission_size(sizeof(byte_array))];
    uint8_t decoded_byte_array[as_data_size(sizeof(encoded_byte_array))];

    encode(byte_array, encoded_byte_array);
    decode(encoded_byte_array, decoded_byte_array);

    for (size_t i = 0; i < sizeof(byte_array); i++) {
        if (byte_array[i] != decoded_byte_array[i]) {
            cout << "Error, encode/decode, line:" << __LINE__ << endl;
            return 1;
        }
    }

    nsize heap_byte_array_size = 8;
    const uint8_t* heap_byte_array = new const uint8_t[heap_byte_array_size] { 1, 2, 3, 4, 5, 6, 7, 8 };

    nsize heap_encoded_byte_array_size = as_transmission_size(heap_byte_array_size);
    uint8_t* heap_encoded_byte_array = new uint8_t[heap_encoded_byte_array_size];

    nsize heap_decoded_byte_array_size = as_data_size(heap_encoded_byte_array_size);
    uint8_t* heap_decoded_byte_array = new uint8_t[heap_decoded_byte_array_size];

    encode(
        heap_byte_array,
        heap_byte_array + (int)heap_byte_array_size,
        heap_encoded_byte_array,
        heap_encoded_byte_array + (int)heap_encoded_byte_array_size
    );

    decode(
        heap_encoded_byte_array,
        heap_encoded_byte_array + (int)heap_encoded_byte_array_size,
        heap_decoded_byte_array,
        heap_decoded_byte_array + (int)heap_decoded_byte_array_size
    );

    if (heap_byte_array_size != heap_decoded_byte_array_size) {
        cout << "Error, size mismatch, line:" << __LINE__ << endl;
        return 1;
    }

    for (nsize i = 0; i < heap_byte_array_size; i++) {
        if (heap_byte_array[(int)i] != heap_decoded_byte_array[(int)i]) {
            cout << "Error, encode/decode, line:" << __LINE__ << endl;
            return 1;
        }
    }

    delete[] heap_byte_array;
    delete[] heap_encoded_byte_array;
    delete[] heap_decoded_byte_array;

    cout << "Success!" << endl;
    return 0;
}
