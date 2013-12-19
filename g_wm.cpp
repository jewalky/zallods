#include "g_wm.h"
#include "f_256.h"
#include "v_main.h"
#include "g_main.h"
#include "r_main.h"
#include "r_blur.h"
#include "c_main.h"
#include "g_wmex.h"

namespace WM
{
    std::vector<Window*> Windows;
    Sprite_256* sLM = NULL;
}

void WM_Init()
{
    WM::sLM = new Sprite_256("graphics/interface/lm.256");
}

void WM_Quit()
{
    if(WM::sLM != NULL)
        delete WM::sLM;
    WM::sLM = NULL;
}

bool WM_Process(const Event& e)
{
    if(WM::Windows.size())
    {
        WM::Windows.back()->process(e);
        return true;
    }

    return false;
}

void WM_AddWindow(Window* wnd)
{
    WM_DelWindow(wnd);
    WM::Windows.push_back(wnd);
}

void WM_DelWindow(Window* wnd)
{
    for(size_t i = 0; i < WM::Windows.size(); i++)
    {
        if(WM::Windows[i] == wnd)
        {
            WM::Windows.erase(WM::Windows.begin()+i);
            i--;
        }
    }
}

bool WM_Display()
{
    if(WM::Windows.size())
    {
        WM::Windows.back()->tick();
        WM::Windows.back()->display();
        return true;
    }

    return false;
}

Window::Window(Window* parent, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    mRect.x = x;
    mRect.y = y;
    mRect.w = w;
    mRect.h = h;
    mParent = parent;
    mInvalidated = true;
}

bool Window::isInvalidated()
{
    return mInvalidated;
}

void Window::invalidate()
{
    mInvalidated = true;
    if(mParent != NULL)
        mParent->invalidate();
}

void Window::show()
{
    WM_AddWindow(this);

    while(!G_IsExiting())
    {
        G_MainLoopStep();
        SDL_Delay(1);
    }
}

void Window::hide()
{
    WM_DelWindow(this);
}

void Window::close()
{
    hide();
    delete this;
}

void Window::addChild(Window* wnd)
{
    delChild(wnd);
    mChildren.push_back(wnd);
}

void Window::delChild(Window* wnd)
{
    for(size_t i = 0; i < mChildren.size(); i++)
    {
        if(mChildren[i] == wnd)
        {
            mChildren.erase(mChildren.begin()+i);
            i--;
        }
    }
}

void Window::process(const Event& e)
{

}

void Window::display()
{

}

void Window::tick()
{
    for(size_t i = 0; i < mChildren.size(); i++)
        mChildren[i]->tick();
}

const SDL_Rect& Window::getRect()
{
    return mRect;
}

ToplevelWindow::ToplevelWindow(uint16_t w, uint16_t h) :
    Window(NULL, 0, 0, 0, 0)
{
    mRect.w = w * 96;
    mRect.h = h * 64;
    mRect.x = (r_width/2)-(mRect.w/2);
    mRect.y = (r_height/2)-(mRect.h/2);
    mOldPicture = NULL;
}

void ToplevelWindow::display()
{
    if(isInvalidated())
    {
        int16_t shadowOffs = 6;

        R_SetTarget(rt_back);

        SDL_Rect updateRect = mRect;
        if(!mAlreadyShown)
        {
            updateRect.x -= 48;
            updateRect.y -= 48;
            updateRect.w += 48*2+shadowOffs;
            updateRect.h += 48*2+shadowOffs;
        }
        else
        {
            updateRect.x -= 36;
            updateRect.y -= 36;
            updateRect.w += 36*2;
            updateRect.h += 36*2;
        }

        R_SetClip(updateRect);

        WM::sLM->displayColored(mRect.x+mRect.w+shadowOffs, mRect.y-48+shadowOffs, 3, 0, 0, 0, 64);
        WM::sLM->displayColored(mRect.x-48+shadowOffs, mRect.y+mRect.h+shadowOffs, 6, 0, 0, 0, 64);
        WM::sLM->displayColored(mRect.x+mRect.w+shadowOffs, mRect.y+mRect.h+shadowOffs, 8, 0, 0, 0, 64);

        for(uint16_t y = mRect.y; y < mRect.y+mRect.h; y+=64)
        {
            for(uint16_t x = mRect.x; x < mRect.x+mRect.w; x+=96)
            {
                if(y == mRect.y)
                {
                    WM::sLM->displayColored(x+shadowOffs, mRect.y+mRect.h+shadowOffs, 7, 0, 0, 0, 64);
                }
            }

            WM::sLM->displayColored(mRect.x+mRect.w+shadowOffs, y+shadowOffs, 5, 0, 0, 0, 64);
        }

        WM::sLM->display(mRect.x-48, mRect.y-48, 1);
        WM::sLM->display(mRect.x+mRect.w, mRect.y-48, 3);
        WM::sLM->display(mRect.x-48, mRect.y+mRect.h, 6);
        WM::sLM->display(mRect.x+mRect.w, mRect.y+mRect.h, 8);

        for(uint16_t y = mRect.y; y < mRect.y+mRect.h; y+=64)
        {
            WM::sLM->display(mRect.x-48, y, 4);

            for(uint16_t x = mRect.x; x < mRect.x+mRect.w; x+=96)
            {
                if(y == mRect.y)
                {
                    WM::sLM->display(x, mRect.y-48, 2);
                    WM::sLM->display(x, mRect.y+mRect.h, 7);
                }

                WM::sLM->display(x, y, 0);
            }

            WM::sLM->display(mRect.x+mRect.w, y, 5);
        }

        for(size_t i = 0; i < mChildren.size(); i++)
            mChildren[i]->display();

        R_UpdateRect(updateRect);

        mInvalidated = false;
        mAlreadyShown = true;
    }
}

void ToplevelWindow::show()
{
    mAlreadyShown = false;
    if(mOldPicture == NULL)
        mOldPicture = new uint32_t[r_width*r_height];
    memcpy(mOldPicture, rt_back->pixels, r_width*r_height*4);
    R_Blur();
    R_FillRect(r_clip, 0, 0, 0, 128);
    R_UpdateRect(r_clip);
    invalidate();
    Window::show();
}

void ToplevelWindow::hide()
{
    Window::hide();
    R_SetTarget(rt_back);
    memcpy(rt_back->pixels, mOldPicture, r_width*r_height*4);
    R_UpdateRect(r_clip);
}

void ToplevelWindow::process(const Event& e)
{
    if(e.type == Event::KeyboardEvent && e.isDown)
    {
        if(e.sym == SDLK_ESCAPE)
        {
            close();
        }
        else
        {
            for(size_t i = 0; i < mChildren.size(); i++)
                mChildren[i]->process(e);
        }
    }
}

MessageBoxWindow::MessageBoxWindow(String text) :
    ToplevelWindow(2, 2)
{
    WLabel* label = new WLabel(this, -8, -8, mRect.w+16, mRect.h+16, text);
    addChild(label);

    WPushButton* button = new WPushButton(this, 0, mRect.h-8, mRect.w, 24, "Ok");
    addChild(button);
}
