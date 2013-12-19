#include "g_main.h"
#include "g_event.h"
#include "g_screen.h"
#include "c_main.h"
#include "r_main.h"
#include "v_main.h"
#include "r_mouse.h"
#include "f_font.h"
#include "g_wm.h"
#include "screens/MainMenu.h"

bool gml_close = false;

bool G_Init()
{
    WM_Init();
    return true;
}

void G_Quit()
{
    WM_Quit();
}

void G_MainLoopStep()
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

        if(!WM_Process(ev))
        {
            if(g_screen)
                g_screen->onEvent(ev);
        }
    }

    R_SetTarget(rt_back);

    if(!WM_Display())
    {
        if(g_screen)
            g_screen->display();
    }

    R_Mouse();
}

bool G_IsExiting()
{
    return gml_close;
}

void G_MainLoop()
{
    LoadCursors();
    LoadFonts();

    gml_close = false;

    R_SetTarget(rt_back);
    R_FillRect(r_clip, 0, 0, 0, 255);

    //G_SetScreen(new ScMainMenu());

    Image* i = new Image("graphics/interface/docs/sheet.bmp");
    i->display(0, 0);
    R_FullUpdate();

    /*ToplevelWindow* wnd = new ToplevelWindow(3, 3);
    wnd->show();*/
    MessageBoxWindow* wnd = new MessageBoxWindow("This is a new messagebox window. It's also multiline btw.");
    wnd->show();

    while(!G_IsExiting())
    {
        G_MainLoopStep();
        SDL_Delay(1);
    }
}
