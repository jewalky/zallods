#include "r_mouse.h"
#include "g_event.h"
#include "f_256.h"
#include "f_16a.h"
#include "f_16.h"
#include "v_main.h"
#include "r_main.h"
#include "c_main.h"

bool rm_oldrecti = true;
SDL_Rect rm_oldrect = {0, 0, 0, 0};

MouseCursor* mc_default = NULL;
MouseCursor* mc_wait = NULL;
MouseCursor* mc_select = NULL;

MouseCursor* rm_cursor = NULL;
uint32_t rm_ticks = 0;
uint32_t rm_frame = 0;

MouseCursor::MouseCursor(String filename, int16_t x, int16_t y, uint32_t delay)
{
    StringList sXp = filename.split(".");
    String sX = sXp[sXp.size()-1].toLowerCase();

    if(sX == "16a")
    {
        sprite = new Sprite_16A(filename);
    }
    else if(sX == "16")
    {
        sprite = new Sprite_16(filename);
    }
    else if(sX == "256")
    {
        sprite = new Sprite_256(filename);
    }

    this->x = x;
    this->y = y;
    this->delay = delay;
    hasOwnSprite = true;
}

MouseCursor::MouseCursor(Sprite* spr)
{
    sprite = spr;
    hasOwnSprite = false;

    if(sprite)
    {
        x = sprite->getWidth(0) / 2;
        y = sprite->getHeight(0) / 2;
    }
    else
    {
        x = 0;
        y = 0;
    }

    delay = 0;
}

void LoadCursors()
{
    C_Printf("LoadCursors: Initializing mouse cursors...\n");

    mc_default = new MouseCursor("graphics/cursors/default/sprites.16a", 3, 3, 0);
    mc_wait = new MouseCursor("graphics/cursors/wait/sprites.16a", 16, 16, 40);
    mc_select = new MouseCursor("graphics/cursors/select/sprites.16a", 4, 4, 0);

    R_SetMouse(mc_default);
}

void R_SetMouse(MouseCursor* cur)
{
    if(cur == rm_cursor) return;

    if(rm_cursor != NULL)
    {
        if(!rm_cursor->hasOwnSprite)
            delete rm_cursor;

        rm_cursor = NULL;
    }

    rm_cursor = cur;
    rm_frame = 0;
    rm_ticks = SDL_GetTicks();
}

void R_Mouse()
{
    if(!rm_cursor || !rm_cursor->sprite || rm_oldrecti)
    {
        if(rm_oldrect.x != 0 || rm_oldrect.y != 0 || rm_oldrect.w != 0 || rm_oldrect.h != 0)
        {
            if(!rm_oldrecti) SDL_UpdateRect(rt_main, rm_oldrect.x, rm_oldrect.y, rm_oldrect.w, rm_oldrect.h);
            rm_oldrect.x = 0;
            rm_oldrect.y = 0;
            rm_oldrect.w = 0;
            rm_oldrect.h = 0;
        }

        rm_oldrecti = false;
        return;
    }

    int32_t nX = Events.getMouseX();
    int32_t nY = Events.getMouseY();

    SDL_Rect newRect;
    newRect.x = nX-rm_cursor->x;
    newRect.y = nY-rm_cursor->y;
    newRect.w = 32;
    newRect.h = 32;

    bool isNull = false;
    if(rm_oldrect.x == 0 && rm_oldrect.y == 0 && rm_oldrect.w == 0 && rm_oldrect.h == 0)
        isNull = true;

    SDL_Rect nOldRect = rm_oldrect;

    R_SetTarget(rt_main);

    R_EndUpdate(false);
    if(!isNull) SDL_BlitSurface(rt_back, &nOldRect, rt_main, &nOldRect);
    SDL_BlitSurface(rt_back, &newRect, rt_main, &newRect);

    rm_cursor->sprite->display(nX-rm_cursor->x, nY-rm_cursor->y, rm_frame);

    SDL_Flip(rt_main);

    rm_oldrect = newRect;
}
