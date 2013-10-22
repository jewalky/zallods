#include "f_16a.h"
#include "v_main.h"
#include "c_main.h"
#include "f_file.h"

Sprite_16A::Sprite_16A(String filename)
{
    if(!rt_main)
        C_Fatal("Sprite_16A::Sprite_16A: Couldn't load \"%s\" (video not initialized yet).\n", filename.c_str());

    File f;
    if(!f.open(filename) || !f.length())
        C_Fatal("Sprite_16A::Sprite_16A: Couldn't open \"%s\".\n", filename.c_str());

    f.seek(f.length()-4);
    uint32_t cnt = 0;

    f.read(&cnt, 4);
    cnt &= 0x7FFFFFFF;

    f.seek(0);
    f.read(mPalette, 1024);

    mFrames.resize(cnt);
    for(uint32_t i = 0; i < cnt; i++)
    {
        uint32_t iw = 0, ih = 0, isz = 0;
        f.read(&iw, 4);
        f.read(&ih, 4);
        f.read(&isz, 4);

        if(!isz)
            C_Fatal("Sprite_16A::Sprite_16A: Size of frame %u is NULL.\n", i);

        uint8_t* data = new uint8_t[isz];
        f.read(data, isz);

        uint8_t* pdata = new uint8_t[iw * ih * 2];
        uint16_t* upixels = (uint16_t*)pdata;
        memset(upixels, 0, iw*ih*2);

        uint8_t* pixels = data;

        uint32_t pos = 0;
        uint32_t pixelpos = 0;

        while(pos < isz)
        {
            uint8_t fbyte = *pixels++;
            uint8_t sbyte = *pixels++;

            if(sbyte)
            {
                if(sbyte == 64)
                {
                    pixelpos += fbyte * iw;
                    pos += 2;
                    upixels += fbyte * iw;
                }
                else
                {
                    pixelpos += fbyte;
                    pos += 2;
                    upixels += fbyte;
                }
            }
            else
            {
                for(int i = 0; i < fbyte; i++)
                {
                    uint16_t ss = *(uint16_t*)(pixels+i*2);
                    uint8_t alpha = (((ss & 0xFF00) >> 9) & 0x0F)+(((ss & 0xFF00) >> 5) & 0xF0);
                    uint8_t color = ((ss & 0xFF00) >> 1)+((ss & 0x00FF) >> 1);
                    ss = alpha;
                    ss <<= 8; ss |= color;
                    upixels[i] = ss;
                }

                pixelpos += fbyte;
                pos += 2 * fbyte + 2;
                upixels += fbyte;
                pixels += 2 * fbyte;
            }
        }

        delete[] data;

        mFrames[i].width = iw;
        mFrames[i].height = ih;
        mFrames[i].data = pdata;
        mFrames[i].dataSize = isz;
    }

    f.close();
}

void Sprite_16A::display(int16_t x, int16_t y, uint32_t num)
{
    displayColored(x, y, num, 255, 255, 255, 255);
}

void Sprite_16A::displayColored(int16_t x, int16_t y, uint32_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if(num >= mFrames.size()) return;
    if(!r_target) return;

    int16_t rx = x;
    int16_t ry = y;
    int16_t rix = 0;
    int16_t riy = 0;
    uint16_t rw = getWidth(num);
    uint16_t rh = getHeight(num);

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
    uint16_t* pixels = (uint16_t*)mFrames[num].data;

    uint32_t addpre_in = rix;
    uint32_t addpost_in = getWidth(num) - (rix+rw);

    uint32_t addpre = rx;
    uint32_t addpost = r_target->w - (rx+rw);

    pixels += riy * getWidth(num);
    upixels += ry * r_target->w;
    for(uint32_t y = 0; y < rh; y++)
    {
        pixels += addpre_in;
        upixels += addpre;
        for(uint32_t x = 0; x < rw; x++)
        {
            uint16_t siv = *pixels++;

            uint8_t a = (siv & 0xFF00) >> 8;
            uint8_t palColor = (siv & 0xFF);
            uint32_t iv = mPalette[palColor];

            uint16_t r = (iv & 0x00FF0000) >> 16;
            uint16_t g = (iv & 0x0000FF00) >> 8;
            uint16_t b = (iv & 0x000000FF);

            uint32_t riv = *upixels;

            uint16_t ir = (riv & 0x00FF0000) >> 16;
            uint16_t ig = (riv & 0x0000FF00) >> 8;
            uint16_t ib = (riv & 0x000000FF);

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
        pixels += addpost_in;
    }
}
