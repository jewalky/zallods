#ifndef V_MAIN_H_INCLUDED
#define V_MAIN_H_INCLUDED

#include <SDL/SDL.h>

// The render target used by most drawing functions
extern SDL_Surface* r_target;
extern SDL_Rect r_clip;

// The main drawing surface
extern SDL_Surface* rt_main;
extern SDL_Surface* rt_back;

// Screen dimensions
extern uint16_t r_width;
extern uint16_t r_height;
extern bool r_fullscreen;

bool V_Init();
void V_Quit();

#endif // V_MAIN_H_INCLUDED
