#pragma once

#include <stdint.h>

namespace nstreamcom {
    template <typename buffer_it>
    class buffered_decoder {
    public:
        buffered_decoder(buffer_it buffer_begin, buffer_it buffer_end)
            : _begin(buffer_begin), _end(buffer_end), _position(buffer_begin), _previous(buffer_begin), _right_shift(0) {}

        buffer_it begin() {
            return _begin;
        }

        buffer_it position() {
            return _position;
        }

        buffer_it end() {
            return _end;
        }

        void use(buffer_it buffer_begin, buffer_it buffer_end, buffer_it position, buffer_it previous) {
            _begin = buffer_begin;
            _end = buffer_end;
            _position = position;
            _previous = previous;
        }

        void reset() {
            _previous = _begin;
            _position = _begin;
            _right_shift = 0;
        }

        bool next(uint8_t byte, bool last) {
            if (!last) {
                if (_position == _end) {
                    return false;
                }

                *_position = (uint8_t)(byte >> _right_shift);
            }

            if (_right_shift != 0) {
                *_previous |= (uint8_t)(byte << (8 - _right_shift));
            }

            if (_right_shift == 7) {
                _right_shift = 0;
            } else {
                if (_position != _begin) {
                    ++_previous;
                }
                ++_position;
                ++_right_shift;
            }

            return true;
        }

    protected:
        buffer_it _begin;
        buffer_it _end;
        buffer_it _position;
        buffer_it _previous;
        uint8_t _right_shift;
    };
}
