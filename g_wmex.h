#ifndef G_WMEX_H_INCLUDED
#define G_WMEX_H_INCLUDED

#include "g_wm.h"
#include "String.h"
#include "g_event.h"

enum
{
    WLabelEnum          = 0x1000,
    WPushButtonEnum     = 0x2000
};

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
        enum
        {
            Msg_Clicked     = WPushButtonEnum
        };

        Window_Focusable;
        WPushButton(Window* parent, int16_t x, int16_t  y, uint16_t w, uint16_t h, String text);

        virtual void setIdentifier(uint32_t id);

        virtual void display();
        virtual void process(const Event& ev);

        virtual void tick();

    protected:
        String mText;

        bool mHovered;
        bool mClicked;

        uint32_t mId;
};

#endif // G_WMEX_H_INCLUDED
