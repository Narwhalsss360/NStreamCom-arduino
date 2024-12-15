#pragma once
#include "nsize.h"

namespace nstreamcom {
    template <typename data_it, typename encoded_it>
    void encode(data_it data_begin, data_it data_end, encoded_it encoded_begin, encoded_it encoded_end) {
        uint8_t left_shift = 0;
        uint8_t previous_stuffed = 0;

        for (nsize i = 0;
            data_begin != data_end && encoded_begin != encoded_end && i <= MAX_DATA_SIZE;
            ++encoded_begin, i++) {
            left_shift = i % 8;
            previous_stuffed = (uint8_t)(8 - left_shift);

            if (left_shift == 0) {
                *encoded_begin = *data_begin;
            } else {
                *encoded_begin = (uint8_t)(*data_begin >> previous_stuffed);
                ++data_begin;
                if (data_begin != data_end) {
                    *encoded_begin |= *data_begin << left_shift;
                }
            }

            *encoded_begin &= ~(0xFF << DATA_BITS);
        }
    }

    template <typename encoded_it, typename data_it>
    void decode(encoded_it encoded_begin, encoded_it encoded_end, data_it data_begin, data_it data_end) {
        uint8_t right_shift = 0;
        uint8_t left_next = 0;

        for (nsize i = 0;
            encoded_begin != encoded_end && data_begin != data_end && i <= MAX_DATA_SIZE;
            ++data_begin, i++) {
            right_shift = (uint8_t)(i % DATA_BITS);
            left_next = (uint8_t)(DATA_BITS - right_shift);

            if (i != 0 && right_shift == 0) {
                ++encoded_begin;
            }

            *data_begin = (uint8_t)((*encoded_begin & ~(0xFF << DATA_BITS)) >> right_shift);

            ++encoded_begin;
            if (encoded_begin != encoded_end) {
                *data_begin |= (*encoded_begin & ~(0xFF << DATA_BITS)) << left_next;
            }
        }
    }
}