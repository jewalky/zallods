#ifndef F_BMP_H_INCLUDED
#define F_BMP_H_INCLUDED

#include <SDL/SDL.h>
#include "String.h"
#include <stdint.h>

class Image
{
    public:
        enum
        {
            OutOfRange = 0xFFFFFFFF,
            NoColorKey = 0xFEEDCAFE
        };

        Image(String filename);
        ~Image();

        void display(int16_t x, int16_t y);
        // note that colorkey is applied using 16-bit mask (0x00101010)
        void displayEx(int16_t x, int16_t y, int16_t inx, int16_t iny, uint16_t w, uint16_t h, uint32_t colorkey = NoColorKey);

        uint32_t getPixelAt(int16_t x, int16_t y);
        uint32_t* getPixels();

        uint16_t getWidth();
        uint16_t getHeight();

    private:
        SDL_Surface* mSurface;
};

#endif // F_BMP_H_INCLUDED
