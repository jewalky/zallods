#ifndef G_WMEX_H_INCLUDED
#define G_WMEX_H_INCLUDED

#include "g_wm.h"
#include "String.h"
#include "g_event.h"

class WLabel : public Window
{
    public:
        WLabel(Window* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, String text);

        virtual void display();

    protected:
        String mText;
};

class WPushButton : public Window
{
    public:
        WPushButton(Window* parent, int16_t x, int16_t  y, uint16_t w, uint16_t h, String text);

        virtual void display();
        virtual void process(const Event& ev);

        virtual void tick();

    protected:
        String mText;

        bool mHovered;
        bool mClicked;
};

#endif // G_WMEX_H_INCLUDED
