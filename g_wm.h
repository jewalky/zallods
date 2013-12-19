#ifndef G_WM_H_INCLUDED
#define G_WM_H_INCLUDED

#include <SDL/SDL.h>
#include <vector>
#include "String.h"
#include "g_event.h"

class Window
{
    public:
        Window(Window* parent, int16_t x, int16_t y, uint16_t w, uint16_t h);

        virtual void show();
        virtual void hide();
        virtual void close();

        void addChild(Window* w);
        void delChild(Window* w);

        virtual void process(const Event& e);
        virtual void display();

        bool isInvalidated();
        void invalidate();

        virtual void tick();

        const SDL_Rect& getRect();

    protected:
        Window* mParent;
        SDL_Rect mRect;

        bool mInvalidated;

        std::vector<Window*> mChildren;
};

class ToplevelWindow : public Window
{
    public:
        ToplevelWindow(uint16_t w, uint16_t h);

        virtual void display();

        virtual void show();
        virtual void hide();

        virtual void process(const Event& e);

    protected:
        bool mAlreadyShown;
        uint32_t* mOldPicture;
};

class MessageBoxWindow : public ToplevelWindow
{
    public:
        MessageBoxWindow(String text);
};

void WM_Init();
void WM_Quit();
bool WM_Process(const Event& e);
void WM_AddWindow(Window* wnd);
void WM_DelWindow(Window* wnd);
bool WM_Display();

#endif // G_WM_H_INCLUDED
