#include "f_sprite.h"
#include <cstdlib>

Sprite::Sprite()
{
    // ?
}

Sprite::~Sprite()
{
    for(size_t i = 0; i < mFrames.size(); i++)
    {
        if(mFrames[i].data)
            delete[] mFrames[i].data;
        mFrames[i].data = NULL;
    }

    mFrames.clear();
}

uint16_t Sprite::getWidth(uint32_t num)
{
    if(num >= mFrames.size()) return 0;
    return mFrames[num].width & 0xFFFF;
}

uint16_t Sprite::getHeight(uint32_t num)
{
    if(num >= mFrames.size()) return 0;
    return mFrames[num].height & 0xFFFF;
}
