#ifndef G_WM_H_INCLUDED
#define G_WM_H_INCLUDED

#include <SDL/SDL.h>
#include <vector>
#include "String.h"
#include "g_event.h"

#define Window_Focusable inline virtual bool isFocusable() { return true; }

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

        inline virtual bool isFocusable() { return false; }

        const SDL_Rect& getRect();

        void resetTabOrder();
        bool advanceTabOrder();

        bool isFocused();
        void focus();
        void unfocus();

        virtual void sendMessage(uint32_t type, uint32_t value, void* data);

    protected:
        Window* mParent;
        SDL_Rect mRect;

        bool mInvalidated;

        std::vector<Window*> mChildren;

        Window* mTabChild;
        bool mFocused;
};

class ToplevelWindow : public Window
{
    public:
        Window_Focusable;
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
        enum MessageBoxType
        {
            Type_Ok,
            Type_YesNo,
            Type_OkCancel,
            Type_AbortRetryIgnore
        };

        MessageBoxWindow(String text, MessageBoxType type);

        virtual void sendMessage(uint32_t type, uint32_t value, void* data);

        uint32_t getResult();

    private:
        uint32_t mResult;
};

void WM_Init();
void WM_Quit();
bool WM_Process(const Event& e);
void WM_AddWindow(Window* wnd);
void WM_DelWindow(Window* wnd);
bool WM_WindowExists(Window* wnd);
bool WM_Display();

#endif // G_WM_H_INCLUDED
