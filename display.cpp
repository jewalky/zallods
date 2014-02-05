#include "config.hpp"
#include "display.hpp"
#include "sys_graphics.hpp"
#include "events.hpp"
#include "core.hpp"
#include <cmath>
#include "image.hpp"

namespace display
{
    SDL_Surface* surface = 0;
    uint32_t surface_id = 0;

    bool init()
    {
        if(surface)
        {
            SDL_Quit();
            SDL_FreeSurface(surface);
        }

        if(SDL_Init(SDL_INIT_VIDEO) != 0)
            return false;

        surface = SDL_SetVideoMode(config::display::width, config::display::height, 32, SDL_OPENGL);
        if(!surface)
        {
            SDL_Quit();
            return false;
        }

#ifdef _WIN32 // Windows implementation destroys GL context in the process
        surface_id++;
#else
        surface_id = 1;
#endif

        GLenum glew_err = glewInit();
        if(glew_err != GLEW_OK)
        {
            SDL_FreeSurface(surface);
            SDL_Quit();
            return false;
        }

        if(!GLEW_VERSION_2_0)
        {
            core::printf(" - OpenGL 2.0 is not supported.%n");
            SDL_FreeSurface(surface);
            SDL_Quit();
            return false;
        }

        SDL_EnableUNICODE(true);

        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glEnable(GL_BLEND);

        project(0, 0, config::display::width, config::display::height);

        return true;
    }

    Image* img = 0;

    float loc = 0;
    void refresh()
    {
        if(!surface)
            return;
        SDL_Event e;
        while(SDL_PollEvent(&e))
            events::eat_event(e);

        glClear(GL_COLOR_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



        SDL_GL_SwapBuffers();
    }

    uint32_t context_id()
    {
        return surface_id;
    }

    void project(int32_t x, int32_t y, uint16_t w, uint16_t h)
    {
        glViewport(0, 0, config::display::width, config::display::height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(x, x+w, config::display::height-y, config::display::height-(y+h), 0, 1);
        glMatrixMode(GL_MODELVIEW);
    }

    #define PI 3.14159265359
    #define POINTS 16
    #define LEVELS 16

    // POINTS = 16, LEVELS = 16, lens_distort(x, y, 32, false, 3);
    // приблизительно похоже на искажения, получающиеся при выстреле летучей мыши
    // остальное тоже можно зачем-нибудь использовать.
    GLuint lens_distort_texture = 0;
    uint32_t lens_distort_context = 0;
    void lens_distort(int16_t x, int16_t y, uint16_t w, bool inverted, float power)
    {
        if(w > 1024)
            return;

        // note that it's somewhat slow
        // >6 distortions in one frame is not recommended

        glFinish();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glFinish();

        GLubyte* pixels = new GLubyte[w*w*4];
        memset(pixels, 0, w*w*4);
        glReadPixels(x, (config::display::height-y)-w+1, w, w, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glFinish();

        if(lens_distort_context != surface_id)
        {
            glGenTextures(1, &lens_distort_texture);
            lens_distort_context = surface_id;
        }

        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, lens_distort_texture);
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 4, w, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

        delete[] pixels;

        int16_t Pxp[POINTS+1];
        int16_t Pyp[POINTS+1];

        int16_t PTxp[POINTS+1];
        int16_t PTyp[POINTS+1];

        int16_t xp[POINTS+1];
        int16_t yp[POINTS+1];

        int16_t Txp[POINTS+1];
        int16_t Typ[POINTS+1];

        float lev = 1.0;
        float left = w;

        float levels[LEVELS];
        for(int j = 0; j < LEVELS; j++)
        {
            lev /= 2.0;
            left -= (float)w * lev;
            levels[j] = w * lev;
        }

        for(int j = 0; j < LEVELS; j++)
        {
            int16_t xcenter = w/2;

            float deltaA = (2*PI)/POINTS;
            float angle = 0;


            for(int i = 0; i < POINTS; i++)
            {
                float s = sin(angle);
                float c = cos(angle);

                float Texp = levels[j];

                float xnew = Texp * c;
                float ynew = Texp * s;

                float _pow = xcenter * j * power;

                float Xdist = (inverted ? (_pow) : (xcenter - _pow));

                float Txnew = Xdist * c;
                float Tynew = Xdist * s;

                xp[i] = xnew+xcenter;
                yp[i] = ynew+xcenter;

                Txp[i] = Txnew+xcenter;
                Typ[i] = w-(Tynew+xcenter);

                angle += deltaA;
            }

            xp[POINTS] = xp[0];
            yp[POINTS] = yp[0];
            Txp[POINTS] = Txp[0];
            Typ[POINTS] = Typ[0];

            if(!j)
            {
                for(int i = 0; i < POINTS+1; i++)
                {
                    Pxp[i] = PTxp[i] = xcenter;
                    Pyp[i] = PTyp[i] = xcenter;
                }
            }

            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, lens_distort_texture);
            glBegin(GL_QUADS);
            for(int i = 0; i < POINTS; i++)
            {
                glColor4ub(255, 255, 255, 255);

                glTexCoord2i(PTxp[i+1], PTyp[i+1]);
                glVertex2i(x+Pxp[i+1], y+Pyp[i+1]);
                glTexCoord2i(PTxp[i], PTyp[i]);
                glVertex2i(x+Pxp[i], y+Pyp[i]);

                glTexCoord2i(Txp[i], Typ[i]);
                glVertex2i(x+xp[i], y+yp[i]);
                glTexCoord2i(Txp[i+1], Typ[i+1]);
                glVertex2i(x+xp[i+1], y+yp[i+1]);
            }
            glEnd();

            for(int i = 0; i < POINTS+1; i++)
            {
                PTxp[i] = Txp[i];
                PTyp[i] = Typ[i];
                Pxp[i] = xp[i];
                Pyp[i] = yp[i];
            }
        }

        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
    }

    void abort()
    {
        if(surface)
        {
            SDL_FreeSurface(surface);
            SDL_Quit();
        }
    }
}
