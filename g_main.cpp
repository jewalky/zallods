#include "g_main.h"
#include "g_event.h"
#include "c_main.h"
#include "r_main.h"
#include "v_main.h"
#include "r_mouse.h"
#include "f_bmp.h"

bool G_Init()
{
    return false;
}

void G_Quit()
{

}

void G_MainLoop()
{
    LoadCursors();

    bool gml_close = false;
    static Image* img = NULL;
    if(!img) img = new Image("locale/ru/graphics/mainmenu/menu_.bmp");

    R_SetTarget(rt_main);
    R_FillRect(r_clip, 0, 0, 0, 255); // clear the view just in case
    img->display(0, 0);
    R_FullUpdate();

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

        R_SetTarget(rt_main);

        // do something

        R_Mouse();

        SDL_Delay(1);
    }
}
