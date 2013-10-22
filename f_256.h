#ifndef F_256_H_INCLUDED
#define F_256_H_INCLUDED

#include "f_sprite.h"
#include "String.h"

class Sprite_256 : public Sprite
{
    public:
        Sprite_256(String filename);

        virtual void display(int16_t x, int16_t y, uint32_t num);
        virtual void displayColored(int16_t x, int16_t y, uint32_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    private:
        uint32_t mPalette[256];
};

#endif // F_256_H_INCLUDED
