#include "v_main.h"
#include "main.h"
#include "c_main.h"

SDL_Surface* r_target = NULL;
SDL_Rect r_clip;

SDL_Surface* rt_main = NULL;
SDL_Surface* rt_back = NULL;

uint16_t r_width = 0;
uint16_t r_height = 0;
bool r_fullscreen = true;

bool V_Init()
{
    r_width = 640;
    r_height = 480;

    for(size_t i = 1; i < g_argv.size(); i++)
    {
        if(g_argv[i] == "-640")
        {
            r_width = 640;
            r_height = 480;
        }
        else if(g_argv[i] == "-800")
        {
            r_width = 800;
            r_height = 600;
        }
        else if(g_argv[i] == "-1024")
        {
            r_width = 1024;
            r_height = 768;
        }
        else if(g_argv[i] == "-fullscreen")
        {
            r_fullscreen = true;
        }
        else if(g_argv[i] == "-window")
        {
            r_fullscreen = false;
        }
    }

    C_Printf("V_Init: Initializing SDL...\n");

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        C_Fatal("V_Init: Couldn't initialize SDL.\n");

    uint32_t sdl_flags = SDL_DOUBLEBUF;
    if(r_fullscreen) sdl_flags |= SDL_FULLSCREEN;

    C_Printf("V_Init: Setting up video mode %ux%ux32%s...\n", r_width, r_height, r_fullscreen ? "" : " (windowed)");

    rt_main = SDL_SetVideoMode(r_width, r_height, 32, sdl_flags);
    if(!rt_main)
        C_Fatal("V_Init: Couldn't initialize main surface: %s.\n", SDL_GetError());

    rt_back = SDL_CreateRGBSurface(0, r_width, r_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if(!rt_back)
        C_Fatal("V_Init: Couldn't initialize back surface: %s.\n", SDL_GetError());

    SDL_EnableUNICODE(true);
    SDL_ShowCursor(false);

    return true;
}

void V_Quit()
{
    C_Printf("V_Quit: Shutting down SDL...\n");
    r_target = NULL;

    if(rt_back) SDL_FreeSurface(rt_back);
    rt_back = NULL;

    if(rt_main) SDL_FreeSurface(rt_main);
    rt_main = NULL;

    SDL_Quit();
}
