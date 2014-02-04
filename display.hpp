#ifndef DISPLAY_HPP_INCLUDED
#define DISPLAY_HPP_INCLUDED

#include <cstdint>

namespace display
{
    uint32_t context_id();
    bool init();
    void refresh();
    void project(int32_t x, int32_t y, uint16_t w, uint16_t h);

    void lens_distort(int16_t x, int16_t y, uint16_t w, bool inverted);
}

#endif // DISPLAY_HPP_INCLUDED
