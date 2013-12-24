#include "g_wmex.h"
#include "f_font.h"
#include "r_main.h"
#include "c_main.h"
#include "g_event.h"

WLabel::WLabel(Window* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, String text) :
    Window(parent, x, y, w, h)
{
    mText = text;
}

void WLabel::display()
{
    if(!mParent)
        return;

    const SDL_Rect& parentRec = mParent->getRect();
    SDL_Rect toRect;
    toRect.x = parentRec.x+mRect.x;
    toRect.y = parentRec.y+mRect.y;
    toRect.w = mRect.w;
    toRect.h = mRect.h;

    Font1->display(toRect, mText, Font::Align_Both, 255, 255, 255, 1);
}

WPushButton::WPushButton(Window* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, String text) :
    Window(parent, x, y, w, h)
{
    mText = text;
    mHovered = false;
    mClicked = false;
}

void WPushButton::display()
{
    if(!mParent)
        return;

    const SDL_Rect& parentRec = mParent->getRect();
    SDL_Rect toRect;
    toRect.x = parentRec.x+mRect.x;
    toRect.y = parentRec.y+mRect.y;
    toRect.w = mRect.w;
    toRect.h = mRect.h;

    SDL_Rect textRect = toRect;
    int16_t ypos = (toRect.h/2)-8;
    textRect.y += ypos;
    textRect.h = 16;

    if(mClicked)
    {
        textRect.x += 1;
        textRect.y += 1;
    }

    if(mHovered || mClicked || mFocused)
        Font1->display(textRect, mText, Font::Align_Center, 212, 132, 47, 2-(mClicked?1:0));
    else Font1->display(textRect, mText, Font::Align_Center, 255, 255, 255, 2);

    uint8_t col1_r = 60;
    uint8_t col1_g = 74;
    uint8_t col1_b = 70;

    uint8_t col2_r = 0;
    uint8_t col2_g = 0;
    uint8_t col2_b = 0;

    if(mClicked)
    {
        col2_r = col1_r; col1_r = 0;
        col2_g = col1_g; col1_g = 0;
        col2_b = col1_b; col1_b = 0;
    }

    R_DrawLine(toRect.x+1, toRect.y, toRect.x+toRect.w-1, toRect.y, col1_r, col1_g, col1_b, 255);
    R_DrawLine(toRect.x, toRect.y+1, toRect.x, toRect.y+toRect.h-1, col1_r, col1_g, col1_b, 255);
    R_DrawLine(toRect.x+1, toRect.y+toRect.h, toRect.x+toRect.w-1, toRect.y+toRect.h, col2_r, col2_g, col2_b, 255);
    R_DrawLine(toRect.x+toRect.w, toRect.y+toRect.h-1, toRect.x+toRect.w, toRect.y+1, col2_r, col2_g, col2_b, 255);
    R_DrawLine(toRect.x+1, toRect.y+toRect.h-1, toRect.x+toRect.w-2, toRect.y+toRect.h-1, col2_r, col2_g, col2_b, 128);
    R_DrawLine(toRect.x+toRect.w-1, toRect.y+toRect.h-1, toRect.x+toRect.w-1, toRect.y+1, col2_r, col2_g, col2_b, 128);
}

void WPushButton::setIdentifier(uint32_t id)
{
    mId = id;
}

void WPushButton::process(const Event& ev)
{
    if(ev.type == Event::MouseEvent && ev.isDown && ev.button == 1 && mHovered) // click
    {
        mClicked = true;
        invalidate();
    }
    else if(ev.type == Event::MouseEvent && !ev.isDown && ev.button == 1) // release
    {
        mClicked = false;
        invalidate();
        if(mHovered && mParent) mParent->sendMessage(Msg_Clicked, mId, NULL);
    }
    else if(ev.type == Event::KeyboardEvent && ev.isDown)
    {
        if(ev.sym == SDLK_RETURN)
        {
            if(mParent) mParent->sendMessage(Msg_Clicked, mId, NULL);
            // ...
        }
        else if(ev.sym == SDLK_SPACE)
        {
            mClicked = true;
            invalidate();
        }
    }
    else if(ev.type == Event::KeyboardEvent && !ev.isDown)
    {
        if(ev.sym == SDLK_SPACE)
        {
            mClicked = false;
            invalidate();
            if(mParent) mParent->sendMessage(Msg_Clicked, mId, NULL);
        }
    }
}

void WPushButton::tick()
{
    Window::tick();

    int16_t mX = Events.getMouseX();
    int16_t mY = Events.getMouseY();

    const SDL_Rect& parentRec = mParent->getRect();
    SDL_Rect toRect;
    toRect.x = parentRec.x+mRect.x;
    toRect.y = parentRec.y+mRect.y;
    toRect.w = mRect.w;
    toRect.h = mRect.h;

    bool nowHovered = (mX >= toRect.x && mY >= toRect.y &&
                        mX <= toRect.x+toRect.w && mY <= toRect.y+toRect.h);
    if(mHovered != nowHovered)
    {
        mHovered = nowHovered;
        invalidate();
    }
}
