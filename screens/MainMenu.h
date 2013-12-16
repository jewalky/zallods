#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include "g_screen.h"
#include "f_bmp.h"

class ScMainMenu : public GenericScreen
{
    public:
        ScMainMenu();
        ~ScMainMenu();

        virtual bool load();
        virtual void unload();

        virtual void display();
        virtual void onEvent(const Event& e);

    private:
        Image* mImg_Background;
        Image* mImg_Mask;
        Image* mImg_Buttons[8];
        Image* mImg_ButtonsP[8];
        Image* mImg_Texts[8];

        bool mChangedBg;

        uint8_t mButtonHovered;
        uint8_t mButtonClicked;
};

#endif // MAINMENU_H_INCLUDED
