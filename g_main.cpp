#include "g_main.h"
#include "g_event.h"
#include "g_screen.h"
#include "c_main.h"
#include "r_main.h"
#include "v_main.h"
#include "r_mouse.h"
#include "f_font.h"
#include "screens/MainMenu.h"

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
    LoadFonts();

    R_SetTarget(rt_back);
    R_FillRect(r_clip, 0, 0, 0, 255);

    //G_SetScreen(new ScMainMenu());

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
            if(g_screen)
                g_screen->onEvent(ev);
        }

        R_SetTarget(rt_back);

        if(g_screen)
            g_screen->display();

        SDL_Rect rr;
        rr.x = 32;
        rr.y = 32;
        rr.w = 128;
        rr.h = 16;
        R_FillRect(rr, 0, 0, 0, 255);


        R_Mouse();

        SDL_Delay(1);
    }
}
