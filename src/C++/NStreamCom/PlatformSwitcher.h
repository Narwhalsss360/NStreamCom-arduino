#ifndef PlatformSwitcher_h
#define PlatformSwitcher_h

#if defined(ARDUINO) && ARDUINO >= 100
using __platform_ostream__ = Stream;

#define __platform_ostream__write(stream, byte_array, byte_array_length) #error "Not implemented."
#else
#include <iostream>
using __platform_ostream__ = std::ostream;
#endif // DEBUG

#endif // !PlatformSwitcher_h