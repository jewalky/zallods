#ifndef IMAGE_HPP_INCLUDED
#define IMAGE_HPP_INCLUDED

#include <cstdint>
#include "sys_graphics.hpp"
#include "String.hpp"

class Image
{
    public:
        enum
        {
            NoColorkey = 0x3A5A3A5A,
            OutOfRange  = 0xA5A3A5A3
        };

        Image(String filename, uint32_t colorkey = NoColorkey);
        ~Image();

        void display(int32_t x, int32_t y);
        uint32_t get_width() { return w; };
        uint32_t get_height() { return h; };
        GLuint get_texture() { push_texture(); return texture; }

        uint32_t get_pixel(int32_t x, int32_t y);

        void push_texture();

    private:
        GLuint texture;

        uint32_t context_id;

        uint32_t* pixels;
        uint32_t w;
        uint32_t h;
};

#endif // IMAGE_HPP_INCLUDED
