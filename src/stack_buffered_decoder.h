#pragma once

#include "buffered_decoder.h"
#include <stdlib.h>

namespace nstreamcom {
    template <size_t size>
    class stack_buffered_decoder : public buffered_decoder<uint8_t*> {
    public:
        using buffer = uint8_t[size];

        stack_buffered_decoder()
            : buffered_decoder<uint8_t*>(_buffer, _buffer + size) {}

        buffer& array() {
            return _buffer;
        }

        const buffer& array() const {
            return _buffer;
        }

    protected:
        buffer _buffer;
    };
}
