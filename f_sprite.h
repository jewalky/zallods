#ifndef F_SPRITE_H_INCLUDED
#define F_SPRITE_H_INCLUDED

#include <vector>
#include <stdint.h>

struct SpriteFrame
{
    uint32_t width;
    uint32_t height;
    uint8_t* data;
    uint32_t dataSize;
};

class Sprite
{
    public:
        Sprite();
        ~Sprite();

        virtual void display(int16_t x, int16_t y, uint32_t num) = 0;
        virtual void displayColored(int16_t x, int16_t y, uint32_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

        uint16_t getWidth(uint32_t num);
        uint16_t getHeight(uint32_t num);

        uint32_t length();

    protected:
        std::vector<SpriteFrame> mFrames;
};

#endif // F_SPRITE_H_INCLUDED
