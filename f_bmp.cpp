#include "f_bmp.h"
#include "f_file.h"
#include "c_main.h"
#include "v_main.h"

Image::Image(String filename)
{
    if(!rt_main)
        C_Fatal("Image::Image: Couldn't load \"%s\" (video not initialized yet).\n", filename.c_str());

    File f;
    if(!f.open(filename) || !f.length())
        C_Fatal("Image::Image: Couldn't open \"%s\".\n", filename.c_str());

    uint8_t* fCc = new uint8_t[f.length()];
    f.read(fCc, f.length());
    SDL_RWops* rw = SDL_RWFromMem(fCc, f.length());
    f.close();

    SDL_Surface* pSurface = SDL_LoadBMP_RW(rw, 0);
    if(!pSurface)
        C_Fatal("Image::Image: Couldn't load \"%s\" (%s).\n", filename.c_str(), SDL_GetError());

    SDL_FreeRW(rw);

    mSurface = SDL_DisplayFormat(pSurface);
    SDL_FreeSurface(pSurface);

    if(!mSurface)
        C_Fatal("Image::Image: Couldn't convert \"%s\" (%s).\n", filename.c_str(), SDL_GetError());

    f.close();
}

Image::~Image()
{
    if(mSurface) SDL_FreeSurface(mSurface);
    mSurface = NULL;
}

void Image::display(int16_t x, int16_t y)
{
    displayEx(x, y, 0, 0, getWidth(), getHeight(), NoColorKey);
}

void Image::displayEx(int16_t x, int16_t y, int16_t inx, int16_t iny, uint16_t w, uint16_t h, uint32_t colorkey)
{
    if(!mSurface) return;
    if(!r_target) return;

    int16_t rx = x;
    int16_t ry = y;
    int16_t rix = inx;
    int16_t riy = iny;
    uint16_t rw = w;
    uint16_t rh = h;

    if(rix < 0) rix = 0;
    if(riy < 0) riy = 0;
    if(rix+rw > getWidth()) rw -= (rix+rw)-getWidth();
    if(riy+rh > getHeight()) rh -= (riy+rh)-getHeight();

    if(rx > r_clip.x+r_clip.w) return;
    if(ry > r_clip.y+r_clip.h) return;
    if(rx+rw < 0 ||
       ry+rh < 0) return;

    if(rx < r_clip.x)
    {
        rw -= (r_clip.x) - rx;
        rix += (r_clip.x) - rx;
        rx = r_clip.x;
    }
    if(ry < r_clip.y)
    {
        rh -= (r_clip.y) - ry;
        riy += (r_clip.y) - ry;
        ry = r_clip.y;
    }
    if(rx+rw > r_clip.x+r_clip.w)
        rw -= ((rx+rw)-(r_clip.x+r_clip.w));
    if(ry+rh > r_clip.y+r_clip.h)
        rh -= ((ry+rh)-(r_clip.y+r_clip.h));

    if(!rw) return;
    if(!rh) return;

    uint32_t* upixels = (uint32_t*)r_target->pixels;
    uint32_t* pixels = (uint32_t*)mSurface->pixels;

    uint32_t addpre_in = rix;
    uint32_t addpost_in = getWidth() - (rix+rw);

    uint32_t addpre = rx;
    uint32_t addpost = r_target->w - (rx+rw);

    pixels += riy * getWidth();
    upixels += ry * r_target->w;
    for(uint32_t y = 0; y < rh; y++)
    {
        pixels += addpre_in;
        upixels += addpre;
        for(uint32_t x = 0; x < rw; x++)
        {
            uint32_t iv = *pixels++;

            if(colorkey != NoColorKey && ((iv & 0x00101010) == (colorkey & 0x00101010)))
            {
                upixels++;
                continue;
            }

            uint16_t ir = (iv & 0x00FF0000) >> 16;
            uint16_t ig = (iv & 0x0000FF00) >> 8;
            uint16_t ib = (iv & 0x000000FF);

            iv = 0xFF00;
            iv |= ir; iv <<= 8;
            iv |= ig; iv <<= 8;
            iv |= ib;

            *upixels++ = iv;
        }
        upixels += addpost;
        pixels += addpost_in;
    }
}

uint16_t Image::getWidth()
{
    if(!mSurface) return 0;
    return mSurface->w;
}

uint16_t Image::getHeight()
{
    if(!mSurface) return 0;
    return mSurface->h;
}

uint32_t Image::getPixelAt(int16_t x, int16_t y)
{
    if(x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return OutOfRange;
    uint32_t* upixels = (uint32_t*)mSurface->pixels;
    return *(upixels + y * getWidth() + x);
}
