#include "g_main.h"
#include "g_event.h"
#include "c_main.h"
#include "r_main.h"
#include "v_main.h"
#include "f_16a.h"

bool G_Init()
{
    return false;
}

void G_Quit()
{

}

void G_MainLoop()
{
    bool gml_close = false;
    while(!gml_close)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
            Events.eatEvent(e);

        Events.tick();

        Event ev;
        while(Events.getEvent(ev))
        {
            if(ev.type == Event::Quit)
                gml_close = true;
        }

        R_SetTarget(rt_back);
        R_FillRect(r_clip, 0, 0, 0, 255); // clear the view just in case

        // do something
        static Sprite_16A* spr = NULL;
        if(!spr) spr = new Sprite_16A("graphics/cursors/default/sprites.16a");
        spr->display(32, 32, 0);

        R_FullBlit();

        SDL_Delay(1);
    }
}
