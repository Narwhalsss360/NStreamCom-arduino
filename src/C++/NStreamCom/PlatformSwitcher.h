#ifndef PlatformSwitcher_h
#define PlatformSwitcher_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
using __platform_ostream__ = Stream;
#define __platform_ostream__write(stream, byte) stream.write(byte)
#define __platform_ostream_write_bytes(stream, byteArray, length) stream.write(byteArray, length)
#define __platform_sleeper(ms) delay(ms)
#else //Assume STL is available
#include <iostream>
using __platform_ostream__ = std::ostream;
#define __platform_ostream__write(stream, byte) (stream.out << byte)
#define __platform_ostream_write_bytes(stream, byteArray, length) { for (uint32_t i_##byteArray = 0; i_##byteArray < length; i_##byteArray++) { __platform_ostream__write(stream, byteArray[i_##byteArray]); } }
#include <chrono>
#include <thread>
#define __platform_sleeper(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#endif

#endif // !PlatformSwitcher_h