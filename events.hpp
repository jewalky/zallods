#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include <SDL/SDL.h>

namespace events
{
    struct Event
    {
        enum
        {
            Ev_KeyboardEvent,
            Ev_TextEvent,
            Ev_MouseEvent,
            Ev_QuitEvent,
            Ev_UserEvent,
            Ev_NoneEvent
        } type;

        // text event, keyboard event
        uint8_t chr;
        uint8_t sym;
        bool is_down; // also mouse event
        bool is_repeated;

        // mouse event
        uint8_t button;
        bool wheel_up;
        bool wheel_down;

        // user event
        uint32_t u_dword1;
        uint32_t u_dword2;
        uint16_t u_word;
        void* u_ptr;

        Event()
        {
            type = Ev_NoneEvent;
            chr = 0;
            sym = 0;
            is_down = false;
            is_repeated = false;
            button = 0;
            wheel_up = false;
            wheel_down = false;
            u_dword1 = 0;
            u_dword2 = 0;
            u_word = 0;
            u_ptr = 0;
        }
    };

    void eat_event(const SDL_Event& e);
    bool get_event(Event& ev);
    void tick();

    uint16_t mouse_x();
    uint16_t mouse_y();
}

#endif // EVENTS_HPP_INCLUDED
