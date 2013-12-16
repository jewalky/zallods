#ifndef F_FONT_H_INCLUDED
#define F_FONT_H_INCLUDED

#include <SDL/SDL.h>
#include "String.h"
#include "f_sprite.h"

class Font
{
    public:
        Font(String base_filename, uint32_t lineHeight, uint32_t spaceWidth);
        ~Font();

        enum Align
        {
            Align_Left,
            Align_Right,
            Align_Center,
            Align_Both
        };

        void display(const SDL_Rect& rect, String text, Align align, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

        uint32_t measureWidth(const SDL_Rect& rect, String text, Align align);
        uint32_t measureWidth(uint32_t width, String text, Align align);

    private:
        Sprite* mSprite;
        uint32_t mSizes[224];
        uint32_t mLineHeight;
};

void LoadFonts();

extern Font* Font1;
extern Font* Font2;
extern Font* Font3;
extern Font* Font4;

#endif // F_FONT_H_INCLUDED
