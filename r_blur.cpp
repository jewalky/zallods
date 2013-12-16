#include "r_main.h"
#include "v_main.h"

void R_Blur()
{
    if(!r_target) return;

    uint32_t* upixels = (uint32_t*)r_target->pixels;

    #define gauss_width 7

    uint16_t sumr=0;
    uint16_t sumg=0;
    uint16_t sumb=0;

    int gauss_fact[gauss_width]={1,6,15,20,15,6,1};
    int gauss_sum=64;

    uint32_t* upixels2 = upixels;

    for(int i = 1; i < r_target->w-1; i++)
    {
        for(int j = 1; j<r_target->h-1; j++)
        {
            sumr = 0;
            sumg = 0;
            sumb = 0;

            upixels2 = (upixels+j*r_target->w);

            for(int k = 0; k < gauss_width; k++)
            {
                uint32_t color = *(upixels2+(i-((gauss_width-1)>>1)+k));
                uint8_t r = (color & 0x00FF0000) >> 16;
                uint8_t g = (color & 0x0000FF00) >> 8;
                uint8_t b = (color & 0x000000FF);
                sumr += r * gauss_fact[k];
                sumg += g * gauss_fact[k];
                sumb += b * gauss_fact[k];

            }

            uint32_t tcolor = 0xFF00;
            tcolor |= sumr/gauss_sum; tcolor <<= 8;
            tcolor |= sumg/gauss_sum; tcolor <<= 8;
            tcolor |= sumb/gauss_sum;

            *(upixels2+i) = tcolor;
        }
    }

    for(int i = 6; i < r_target->w - 6; i++)
    {
        for(int j = 6; j < r_target->h - 6; j++)
        {
            sumr = 0;
            sumg = 0;
            sumb = 0;

            for(int k = 0; k < gauss_width; k++)
            {
                uint32_t color = *(upixels+(j-((gauss_width-1)>>1)+k)*r_target->w+i);
                uint8_t r = (color & 0x00FF0000) >> 16;
                uint8_t g = (color & 0x0000FF00) >> 8;
                uint8_t b = (color & 0x000000FF);
                sumr += r * gauss_fact[k];
                sumg += g * gauss_fact[k];
                sumb += b * gauss_fact[k];
            }

            uint32_t tcolor = 0xFF00;
            tcolor |= sumr/gauss_sum; tcolor <<= 8;
            tcolor |= sumg/gauss_sum; tcolor <<= 8;
            tcolor |= sumb/gauss_sum;

            *(upixels+j*r_target->w+i) = tcolor;
        }
    }
}
