#include <nsize.h>
#include <nencode.h>

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
    for (nsize_int i = 0; i < ENCODED_NSIZE_SIZE; i++) {
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

nsize::nsize_int_bytes& nsize::operator=(const nsize::nsize_int_bytes& other_bytes) {
    for (nsize_int i = 0; i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] = (uint8_t)(other_bytes[i] | (0xFF << DATA_BITS));
    }
    return bytes;
}

bool nsize::encoded() const {
    return _is_encoded;
}

nsize::nsize_int_bytes& nsize::encode() {
    if (_is_encoded) {
        return bytes;
    }

    nsize_int_bytes encoded_bytes;

    nstreamcom::encode<uint8_t*, uint8_t*>(
        bytes,
        bytes + sizeof(nsize_int),
        encoded_bytes,
        encoded_bytes + ENCODED_NSIZE_SIZE
    );

    *this = encoded_bytes;

    _is_encoded = true;
    return bytes;
}

nsize_int& nsize::decode() {
    if (!_is_encoded) {
        return (nsize_int&)*this;
    }

    for (nsize_int i = 0; i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] &= ~(0xFF << DATA_BITS);
    }

    nstreamcom::decode<uint8_t*, uint8_t*>(
        bytes,
        bytes + ENCODED_NSIZE_SIZE,
        bytes,
        bytes + sizeof(nsize_int)
    );

    return (nsize_int&)*this;
}

void nsize::fill_trail() {
    for (nsize_int i = sizeof(nsize_int); i < ENCODED_NSIZE_SIZE; i++) {
        bytes[i] = 0;
    }
}
