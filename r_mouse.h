#ifndef R_MOUSE_H_INCLUDED
#define R_MOUSE_H_INCLUDED

#include "f_sprite.h"
#include "String.h"

class MouseCursor
{
    public:
        MouseCursor(String filename, int16_t x, int16_t y, uint32_t delay);
        MouseCursor(Sprite* sprite);

    private:
        Sprite* sprite;
        int16_t x;
        int16_t y;
        int32_t delay;
        bool hasOwnSprite;

        friend void R_SetMouse(MouseCursor*);
        friend void R_Mouse();
};

extern MouseCursor* mc_default;
extern MouseCursor* mc_wait;
extern MouseCursor* mc_select;

void LoadCursors();
void R_SetMouse(MouseCursor* cur);
void R_Mouse();

#endif // R_MOUSE_H_INCLUDED
