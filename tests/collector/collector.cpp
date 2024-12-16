#include <nstreamcom.h>
#include <iostream>
#include <vector>

int main() {
    using std::cout;
    using std::endl;
    using std::vector;
    using nstreamcom::encode_with_size;
    using nstreamcom::as_collected_size;
    using nstreamcom::nsize_int;
    using nstreamcom::buffered_decoder;
    using nstreamcom::stack_buffered_decoder;
    using nstreamcom::collector;
    using nstreamcom::collector_states;

    const char str[] = "Lorem ipsum dolor sit amet, consectetur adipiscing nunc.";
    uint8_t str_encoded_with_size[as_collected_size(sizeof(str))];
    encode_with_size(str, str_encoded_with_size);
    auto byte_collector = collector<stack_buffered_decoder<sizeof(str_encoded_with_size)>>();

    for (uint8_t byte : str_encoded_with_size) {
        byte_collector.collect(byte);
        if (byte_collector.state() == collector_states::MISSING_SIZE) {
            cout << "Error, missing size" << __LINE__ << endl;
            return 1;
        }
        if (byte_collector.state() == collector_states::MISSING_DATA) {
            cout << "Error, missing data" << __LINE__ << endl;
            return 1;
        }
        if (byte_collector.state() == collector_states::BUFFER_FULL) {
            cout << "Error, buffer full" << __LINE__ << endl;
            return 1;
        }
    }

    if (byte_collector.state() != collector_states::COLLECTED) {
        cout << "Error, not collected" << __LINE__ << endl;
        return 1;
    }

    if (byte_collector.next_size() != sizeof(str)) {
        cout << "Error, size mismach" << __LINE__ << endl;
        return 1;
    }

    char (&decoded)[sizeof(str)] = (char(&)[sizeof(str)])byte_collector.decoder().array();

    for (nsize_int i = 0; i < sizeof(str); i++) {
        if (str[i] != decoded[i]) {
            cout << "Error, encode/decode" << __LINE__ << endl;
            return 1;
        }
    }

    vector<uint8_t> buffer;
    auto vector_byte_collector = collector<buffered_decoder<vector<uint8_t>::iterator>>(buffer.begin(), buffer.end());

    for (nsize_int i = 0; i < sizeof(str_encoded_with_size); i++) {
        vector_byte_collector.collect(str_encoded_with_size[i]);
        if (vector_byte_collector.state() == collector_states::BUFFER_FULL) {
            int index = vector_byte_collector.decoder().position() - buffer.begin();
            buffer.resize(buffer.size() + 1);
            vector_byte_collector.decoder().use(
                buffer.begin(),
                buffer.end(),
                buffer.begin() + index,
                buffer.begin() + (index == 0 ? 0 : index - 1)
            );
            i--;
            vector_byte_collector.expanded();
            continue;
        }
        if (vector_byte_collector.state() == collector_states::MISSING_SIZE) {
            cout << "Error, missing size" << __LINE__ << endl;
            return 1;
        }
        if (vector_byte_collector.state() == collector_states::MISSING_DATA) {
            cout << "Error, missing data" << __LINE__ << endl;
            return 1;
        }
        if (vector_byte_collector.state() == collector_states::BUFFER_FULL) {
            cout << "Error, buffer full" << __LINE__ << endl;
            return 1;
        }
    }

    if (vector_byte_collector.state() != collector_states::COLLECTED) {
        cout << "Error, not collected" << __LINE__ << endl;
        return 1;
    }

    if (vector_byte_collector.next_size() != sizeof(str)) {
        cout << "Error, size mismach" << __LINE__ << endl;
        return 1;
    }

    for (nsize_int i = 0; i < sizeof(str); i++) {
        if (str[i] != buffer[i]) {
            cout << "Error, encode/decode" << __LINE__ << endl;
            return 1;
        }
    }

    cout << "Success!" << endl;
    return 0;
}
