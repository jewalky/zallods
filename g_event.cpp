#include "g_event.h"
#include "c_main.h"

class Events Events;

Events::Events()
{
    mEvents.clear();
    mRepeating = false;
    mRepeatingDown = false;
}

void Events::eatEvent(const SDL_Event& e)
{
    if(e.type == SDL_QUIT ||
       (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F4 &&
        ((e.key.keysym.mod & KMOD_ALT) == KMOD_ALT)))
    {
        Event ev;
        ev.type = Event::Quit;
        mEvents.push_front(ev);
    }
    else if(e.type == SDL_KEYDOWN)
    {
        mRepeatingDown = true;
        mRepeating = false;
        mRepeatingE.isDown = true;
        mRepeatingE.sym = e.key.keysym.sym;
        if(e.key.keysym.unicode > 0x20)
        {
            uint16_t uc = e.key.keysym.unicode;
            if(uc < 0x7F) mRepeatingE.chr = uc;
            else if(uc >= 0x410 && uc <= 0x43F)
                mRepeatingE.chr = uc - 0x390;
            else if(uc >= 0x440 && uc <= 0x44F)
                mRepeatingE.chr = uc - 0x360;
            else if(uc == 0x401)
                mRepeatingE.chr = 0xF0;
            else if(uc == 0x451)
                mRepeatingE.chr = 0xF1;
            else mRepeatingE.chr = 0;
        }
        else mRepeatingE.chr = 0;

        if(mRepeatingE.chr)
        {
            Event ev = mRepeatingE;
            ev.type = Event::TextEvent;
            mEvents.push_front(ev);
        }
        else
        {
            Event ev = mRepeatingE;
            ev.type = Event::KeyboardEvent;
            mEvents.push_front(ev);
        }

        mLastTicks = SDL_GetTicks();
    }
    else if(e.type == SDL_KEYUP)
    {
        mRepeatingDown = false;
        mRepeating = false;

        Event ev;
        ev.type = Event::KeyboardEvent;
        ev.isDown = false;
        ev.chr = 0;
        ev.sym = e.key.keysym.sym;
        mEvents.push_front(ev);
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        Event ev;
        ev.type = Event::MouseEvent;
        ev.isDown = true;

        ev.button = 0;
        if(e.button.button == SDL_BUTTON_LEFT)
            ev.button = 1;
        else if(e.button.button == SDL_BUTTON_RIGHT)
            ev.button = 2;
        else if(e.button.button == SDL_BUTTON_MIDDLE)
            ev.button = 3;

        uint16_t bitmask = (ev.button == 0) ? 0 : (1 << (ev.button-1));
        mMouseMask |= bitmask;

        mMouseX = e.button.x;
        mMouseY = e.button.y;

        mEvents.push_front(ev);
    }
    else if(e.type == SDL_MOUSEBUTTONUP)
    {
        Event ev;
        ev.type = Event::MouseEvent;
        ev.isDown = false;

        if(e.button.button == SDL_BUTTON_LEFT)
            ev.button = 1;
        else if(e.button.button == SDL_BUTTON_RIGHT)
            ev.button = 2;
        else if(e.button.button == SDL_BUTTON_MIDDLE)
            ev.button = 3;

        uint16_t bitmask = (ev.button == 0) ? 0 : (1 << (ev.button-1));
        mMouseMask &= ~bitmask;

        mMouseX = e.button.x;
        mMouseY = e.button.y;

        mEvents.push_front(ev);
    }
    else if(e.type == SDL_MOUSEMOTION)
    {
        mMouseX = e.motion.x;
        mMouseY = e.motion.y;
    }
}

void Events::tick()
{
    if(mRepeatingDown)
    {
        if(SDL_GetTicks()-mLastTicks >= 200)
        {
            mRepeating = true;
            mRepeatingDown = false;

            if(mRepeatingE.chr)
            {
                Event e = mRepeatingE;
                e.type = Event::TextEvent;
                mEvents.push_front(e);
            }
            else
            {
                Event e = mRepeatingE;
                e.type = Event::KeyboardEvent;
                mEvents.push_front(e);
            }

            mLastTicks = SDL_GetTicks();
        }
    }
    else if(mRepeating)
    {
        if(SDL_GetTicks()-mLastTicks >= 50)
        {
            if(mRepeatingE.chr)
            {
                Event e = mRepeatingE;
                e.type = Event::TextEvent;
                mEvents.push_front(e);
            }
            else
            {
                Event e = mRepeatingE;
                e.type = Event::KeyboardEvent;
                mEvents.push_front(e);
            }

            mLastTicks = SDL_GetTicks();
        }
    }
}

bool Events::getEvent(Event& ev)
{
    if(mEvents.size())
    {
        ev = mEvents.back();
        mEvents.pop_back();
        return true;
    }

    return false;
}

uint16_t Events::getMouseX()
{
    return mMouseX;
}

uint16_t Events::getMouseY()
{
    return mMouseY;
}

uint16_t Events::getMouseMask()
{
    return mMouseMask;
}
