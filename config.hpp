#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <cstdint>

namespace config
{
    extern bool is_server;

    namespace display
    {
        extern uint16_t width;
        extern uint16_t height;
    }
}

#endif // CONFIG_HPP_INCLUDED
