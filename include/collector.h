#include <stdint.h>
#include "nsize.h"

namespace nstreamcom {
    enum class collector_states : uint8_t {
        COLLECTED,
        MISSING_SIZE,
        MISSING_DATA,
        WAITING_SIZE,
        WAITING_DATA,
        BUFFER_FULL
    };

    template <typename buffered_decoder>
    class collector {
    public:
        template <typename... args_t>
        collector(args_t... args)
            : _decoder(buffered_decoder(args...)),
            _state(collector_states::WAITING_SIZE),
            _last_state(collector_states::WAITING_SIZE),
            _next_size(0) {}

        collector_states state() const {
            return _state;
        }

        void expanded() {
            if (_state != collector_states::BUFFER_FULL) {
                return;
            }
            _state = _last_state;
        }

        void reset() {
            _next_size = 0;
            _decoder.reset();
            _state = collector_states::MISSING_SIZE;
        }

        collector_states collect(uint8_t byte) {
            if (_last_state != _state && _state != collector_states::BUFFER_FULL) {
                _last_state = _state;
            }

            switch (_state)
            {
            case collector_states::COLLECTED:
            case collector_states::MISSING_SIZE:
            case collector_states::MISSING_DATA:
                reset();
            case collector_states::WAITING_SIZE:
                _collect_size_byte(byte);
                break;
            case collector_states::WAITING_DATA:
                _collect_data_byte(byte);
            case collector_states::BUFFER_FULL:
            default:
                break;
            }

            return _state;
        }

        buffered_decoder& decoder() {
            return _decoder;
        }

        nsize next_size() {
            if (_state == collector_states::WAITING_DATA || _state == collector_states::COLLECTED) {
                return _next_size;
            }

            return 0;
        }

    protected:
        buffered_decoder _decoder;
        collector_states _state;
        collector_states _last_state;
        nsize _next_size;

        void _collect_size_byte(uint8_t byte) {
            if ((byte & (1 << 7)) == 0) {
                _state = collector_states::MISSING_SIZE;
                return;
            }

            byte &= ~(0xFF << DATA_BITS);
            bool last = _decoder.position() - _decoder.begin() == sizeof(nsize_int);
            if (!_decoder.next(byte, last)) {
                _state = collector_states::BUFFER_FULL;
                return;
            }

            if (!last) {
                return;
            }

            _next_size = 0;

            auto decoder_begin = _decoder.begin();
            for (nsize_int i = 0; i < sizeof(nsize_int); i++) {
                _next_size.bytes[i] = *decoder_begin;
                ++decoder_begin;
            }
            _decoder.reset();

            _state = (
                _next_size == 0 ?
                collector_states::COLLECTED :
                collector_states::WAITING_DATA
            );
        }

        void _collect_data_byte(uint8_t byte) {
            if ((byte & (1 << 7)) > 0) {
                _state = collector_states::MISSING_DATA;
                return;
            }

            bool last = (nsize_int)(_decoder.position() - _decoder.begin()) == _next_size;
            if (!_decoder.next(byte, last)) {
                _state = collector_states::BUFFER_FULL;
                return;
            }

            if (!last) {
                return;
            }

            _state = collector_states::COLLECTED;
        }
    };
}
