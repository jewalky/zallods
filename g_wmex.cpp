#include "g_wmex.h"
#include "f_font.h"

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

    if(mHovered)
        Font1->display(textRect, mText, Font::Align_Center, 255, 0, 0, 2);
    else Font1->display(textRect, mText, Font::Align_Center, 255, 255, 255, 1);
}

void WPushButton::process(const Event& ev)
{

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
