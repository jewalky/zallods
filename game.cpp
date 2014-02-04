#include "config.hpp"
#include "display.hpp"
#include "game.hpp"
#include "sys_graphics.hpp"
#include "events.hpp"

namespace game
{
    bool exiting = false;

    bool is_exiting()
    {
        return exiting;
    }

    void step()
    {
        if(!config::is_server)
        {
            display::refresh();

            // process events here
            // - events::get_event()
            events::Event ev;
            while(events::get_event(ev))
            {
                if(ev.type == events::Event::Ev_QuitEvent)
                    exiting = true;
            }
        }

        SDL_Delay(1);
    }

    void loop()
    {
        while(!is_exiting())
            step();
    }
}
