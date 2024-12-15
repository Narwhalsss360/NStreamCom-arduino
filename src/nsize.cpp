#include <nsize.h>

using namespace nstreamcom;

nsize::nsize(const nsize_int& size)
    : _is_encoded(false)
{
    *reinterpret_cast<nsize_int*>(bytes) = size;
    fill_trail();
}

nsize::nsize(const nsize_int_bytes& bytes)
    : _is_encoded(true)
{
    for (int8_t i = 0; i < ENCODED_NSIZE_SIZE; i++) {
        this->bytes[i] = bytes[i];
    }
}

nsize::operator nsize_int&() {
    return *reinterpret_cast<nsize_int*>(bytes);
}

nsize::operator const nsize_int&() const {
    return *reinterpret_cast<const nsize_int*>(bytes);
}

nsize& nsize::operator=(const nsize_int& size) {
    *reinterpret_cast<nsize_int*>(bytes) = size;
    fill_trail();
    _is_encoded = false;
    return *this;
}

nsize& nsize::operator=(const nsize& other) {
    for (int8_t i = 0; i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] = other.bytes[i];
    }
    _is_encoded = other._is_encoded;
    return *this;
}

nsize::nsize_int_bytes& nsize::operator=(const nsize::nsize_int_bytes& other_bytes) {
    for (int8_t i = 0; i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] = other_bytes[i];
    }
    return bytes;
}

bool nsize::encoded() const {
    return _is_encoded;
}

nsize::nsize_int_bytes& nsize::encode() {
    return bytes;
}

nsize_int& nsize::decode() {
    return (nsize_int&)*this;
}

void nsize::fill_trail() {
    for (int8_t i = sizeof(nsize_int); i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] = 0;
    }
}
