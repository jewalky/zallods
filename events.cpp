#include <vector>
#include "events.hpp"

namespace events
{
    std::vector<Event> events;

    uint16_t m_x = 0;
    uint16_t m_y = 0;

    uint32_t repeat_ticks = 0;
    uint8_t repeat_chr = 0;
    uint8_t repeat_sym = 0;

    void eat_event(const SDL_Event& e)
    {
        if(e.type == SDL_MOUSEMOTION ||
           e.type == SDL_MOUSEBUTTONDOWN ||
           e.type == SDL_MOUSEBUTTONUP)
        {
            if(e.type == SDL_MOUSEMOTION)
            {
                m_x = e.motion.x;
                m_y = e.motion.y;
            }
            else
            {
                m_x = e.button.x;
                m_y = e.button.y;

                Event ev;
                ev.type = Event::Ev_MouseEvent;
                ev.is_down = (e.type == SDL_MOUSEBUTTONDOWN);
                switch(e.button.button)
                {
                    default:
                        ev.button = 0;
                        break;
                    case SDL_BUTTON_LEFT:
                        ev.button = 1;
                        break;
                    case SDL_BUTTON_RIGHT:
                        ev.button = 2;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        ev.button = 3;
                        break;
                    case SDL_BUTTON_WHEELUP:
                        ev.button = 3;
                        ev.wheel_up = true;
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        ev.button = 3;
                        ev.wheel_down = true;
                        break;
                }

                if(ev.button)
                    events.push_back(ev);
            }
        }
        else if(e.type == SDL_KEYDOWN)
        {
            // generate keydown event
            Event ev;
            ev.type = Event::Ev_KeyboardEvent;
            ev.is_down = true;
            ev.is_repeated = false;
            ev.sym = e.key.keysym.sym;
            events.push_back(ev);

            uint16_t uc = e.key.keysym.unicode;
            if(uc < 0x7F) ev.chr = uc;
            else if(uc >= 0x410 && uc <= 0x43F)
                ev.chr = uc-0x390;
            else if(uc >= 0x440 && uc <= 0x44F)
                ev.chr = uc-0x360;
            else if(uc == 0x401)
                ev.chr = 0xF0;
            else if(uc == 0x451)
                ev.chr = 0xF1;
            else ev.chr = 0;

            if(ev.sym == SDLK_LSHIFT || ev.sym == SDLK_RSHIFT ||
               ev.sym == SDLK_LCTRL || ev.sym == SDLK_RCTRL ||
               ev.sym == SDLK_LALT || ev.sym == SDLK_RALT)
            {
                repeat_sym = 0;
                repeat_chr = 0;
            }
            else
            {
                repeat_sym = ev.sym;
                repeat_chr = ev.chr;
                repeat_ticks = SDL_GetTicks()+500;
            }

            if(ev.chr > 0)
            {
                ev.type = Event::Ev_TextEvent;
                events.push_back(ev);
            }
        }
        else if(e.type == SDL_KEYUP)
        {
            Event ev;
            ev.type = Event::Ev_KeyboardEvent;
            ev.is_down = false;
            ev.is_repeated = false;
            ev.sym = e.key.keysym.sym;
            events.push_back(ev);

            repeat_sym = 0;
            repeat_chr = 0;
        }
        else if(e.type == SDL_QUIT)
        {
            Event ev;
            ev.type = Event::Ev_QuitEvent;
            events.push_back(ev);
        }
    }

    bool get_event(Event& ev)
    {
        if(!events.size())
            return false;

        ev = events[0];
        events.erase(events.begin());
        return true;
    }

    void tick()
    {
        if(SDL_GetTicks()-repeat_ticks > 250)
        {
            if(repeat_sym)
            {
                Event ev;
                ev.type = Event::Ev_KeyboardEvent;
                ev.is_down = true;
                ev.is_repeated = true;
                ev.sym = repeat_sym;
                events.push_back(ev);

                if(repeat_chr)
                {
                    ev.type = Event::Ev_TextEvent;
                    ev.chr = repeat_chr;
                    events.push_back(ev);
                }
            }

            repeat_ticks = SDL_GetTicks();
        }
    }

    uint16_t mouse_x()
    {
        return m_x;
    }

    uint16_t mouse_y()
    {
        return m_y;
    }
}
