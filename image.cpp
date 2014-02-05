#include "image.hpp"
#include "core.hpp"
#include "display.hpp"

Image::Image(String filename, uint32_t colorkey)
{
    context_id = 0;
    pixels = 0;
    w = 0;
    h = 0;
    texture = 0;

    SDL_Surface* srf = SDL_LoadBMP(filename.c_str());
    if(!srf)
    {
        core::printf(" - Image::Image(): Couldn't load file \"%s\".%n", filename);
        core::abort();
    }

    SDL_Surface* ssrf = SDL_CreateRGBSurface(0, srf->w, srf->h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if(!ssrf)
    {
        core::printf(" - Image::Image(): Couldn't convert file \"%s\".%n", filename);
        SDL_FreeSurface(srf);
        core::abort();
    }

    SDL_BlitSurface(srf, NULL, ssrf, NULL);
    SDL_FreeSurface(srf);

    w = ssrf->w;
    h = ssrf->h;

    pixels = new uint32_t[w * h];
    memcpy(pixels, ssrf->pixels, w*h*4);

    SDL_FreeSurface(ssrf);

    for(uint32_t i = 0; i < w * h; i++)
    {
        if((pixels[i] & 0x00F0F0F0) == (colorkey & 0x00F0F0F0))
            pixels[i] = 0;
        else pixels[i] |= 0xFF000000;
    }

    texture = 0;
}

Image::~Image()
{
    if(pixels)
        delete[] pixels;
    if(display::context_id() == context_id)
        glDeleteTextures(1, &texture);
}

void Image::push_texture()
{
    if(display::context_id() == context_id)
        return;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 4, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
    context_id = display::context_id();
}

void Image::display(int32_t x, int32_t y)
{
    push_texture();
    glColor4ub(255, 255, 255, 255);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2i(x, y);
        glTexCoord2i(w, 0);
        glVertex2i(x+w, y);
        glTexCoord2i(w, h);
        glVertex2i(x+w, y+h);
        glTexCoord2i(0, h);
        glVertex2i(x, y+h);
    glEnd();
}
