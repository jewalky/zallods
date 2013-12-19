#ifndef R_MAIN_H_INCLUDED
#define R_MAIN_H_INCLUDED

#include <SDL/SDL.h>
#include <stdint.h>

void R_SetTarget(SDL_Surface* target);
void R_SetClip(int16_t x, int16_t y, uint16_t w, uint16_t h);
void R_SetClip(const SDL_Rect& rec);
void R_FillRect(const SDL_Rect& rec, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void R_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void R_FullUpdate();
void R_UpdateRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void R_UpdateRect(const SDL_Rect& rec);
void R_EndUpdate(bool flip);

#endif // R_MAIN_H_INCLUDED
