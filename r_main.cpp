#include "r_main.h"
#include "v_main.h"
#include <list>

std::list<SDL_Rect> r_updatelist;

void R_SetTarget(SDL_Surface* target)
{
    r_target = target;

    r_clip.x = 0;
    r_clip.y = 0;

    if(target)
    {
        r_clip.w = target->w;
        r_clip.h = target->h;
    }
    else
    {
        r_clip.w = 0;
        r_clip.h = 0;
    }
}

void R_SetClip(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    r_clip.x = x;
    r_clip.y = y;
    r_clip.w = w;
    r_clip.h = h;
}

void R_SetClip(const SDL_Rect& rec)
{
    R_SetClip(rec.x, rec.y, rec.w, rec.h);
}

void R_FillRect(const SDL_Rect& rec, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if(!r_target) return;
    if(!a) return;

    int16_t rx = rec.x;
    int16_t ry = rec.y;
    uint16_t rw = rec.w;
    uint16_t rh = rec.h;

    if(rx < r_clip.x)
    {
        rw -= (r_clip.x) - rx;
        rx = r_clip.x;
    }
    if(ry < r_clip.y)
    {
        rh -= (r_clip.y) - ry;
        ry = r_clip.y;
    }
    if(rx+rw > r_clip.x+r_clip.w)
        rw -= ((rx+rw)-(r_clip.x+r_clip.w));
    if(ry+rh > r_clip.y+r_clip.h)
        rh -= ((ry+rh)-(r_clip.y+r_clip.h));

    uint32_t* upixels = (uint32_t*)r_target->pixels;

    uint32_t addpre = rx;
    uint32_t addpost = r_target->w - (rx+rw);

    upixels += ry * r_target->w;
    for(uint32_t y = 0; y < rh; y++)
    {
        upixels += addpre;
        for(uint32_t x = 0; x < rw; x++)
        {
            uint32_t iv = *upixels;
            uint16_t ir = (iv & 0x00FF0000) >> 16;
            uint16_t ig = (iv & 0x0000FF00) >> 8;
            uint16_t ib = (iv & 0x000000FF);

            if(a < 255)
            {
                float aa = (float)a / 255.0;
                ir = ((float)ir * (1.0-aa) + (float)r * (aa)); if(ir > 255) ir = 255;
                ig = ((float)ig * (1.0-aa) + (float)g * (aa)); if(ig > 255) ig = 255;
                ib = ((float)ib * (1.0-aa) + (float)b * (aa)); if(ib > 255) ib = 255;
            }
            else
            {
                ir = r;
                ig = g;
                ib = b;
            }

            iv = 0xFF00;
            iv |= ir; iv <<= 8;
            iv |= ig; iv <<= 8;
            iv |= ib;

            *upixels++ = iv;
        }
        upixels += addpost;
    }
}

void R_FullUpdate()
{
    if(!rt_back || !rt_main) return;
    SDL_BlitSurface(rt_back, NULL, rt_main, NULL);
    SDL_Flip(rt_main);
}

void R_UpdateRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if(!rt_main) return;
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    R_UpdateRect(r);
}

void R_UpdateRect(const SDL_Rect& rec)
{
    r_updatelist.push_back(rec);
}

void R_EndUpdate(bool flip)
{
    for(std::list<SDL_Rect>::iterator it = r_updatelist.begin(); it != r_updatelist.end(); ++it)
    {
        SDL_Rect& r = (*it);
        SDL_BlitSurface(rt_back, &r, rt_main, &r);
    }

    r_updatelist.clear();
    if(flip) SDL_Flip(rt_main);
}
