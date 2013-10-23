#ifndef G_SCREEN_H_INCLUDED
#define G_SCREEN_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "g_event.h"

class GenericScreen
{
    public:
        GenericScreen();
        ~GenericScreen();

        virtual bool load();
        virtual void unload();

        virtual void display();
        virtual void onEvent(const Event& e);

        void run();

        enum LoadedState
        {
            NotSet = 0,
            Loaded = 1,
            Failed = -1
        };

        LoadedState getLoadedState();

    private:
        bool mDiscard;
        LoadedState mLoadedState;
        SDL_Thread* mThread;
        static int runThread(void* v);

        friend void G_SetScreen(GenericScreen* s);
};

extern GenericScreen* g_screen;

void G_SetScreen(GenericScreen* s);

#endif // G_SCREEN_H_INCLUDED
