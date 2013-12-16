#include "MainMenu.h"
#include "r_mouse.h"
#include "c_main.h"
#include "g_locale.h"
#include "r_main.h"
#include "v_main.h"

ScMainMenu::ScMainMenu()
{
    mChangedBg = true;
    mImg_Background = NULL;
    mImg_Mask = NULL;
    for(uint8_t i = 0; i < 8; i++)
    {
        mImg_Buttons[i] = NULL;
        mImg_ButtonsP[i] = NULL;
        mImg_Texts[i] = NULL;
    }

    mButtonHovered = 0;
    mButtonClicked = 0;
}

ScMainMenu::~ScMainMenu()
{

}

bool ScMainMenu::load()
{
    mImg_Background = new Image("locale/"+g_localestring+"/graphics/mainmenu/menu_.bmp");
    mImg_Mask = new Image("locale/"+g_localestring+"/graphics/mainmenu/menumask.bmp");
    for(uint8_t i = 1; i <= 8; i++)
    {
        mImg_Buttons[i-1] = new Image("locale/"+g_localestring+"/graphics/mainmenu/"+String::format("button%u.bmp", i));
        mImg_ButtonsP[i-1] = new Image("locale/"+g_localestring+"/graphics/mainmenu/"+String::format("button%up.bmp", i));
        mImg_Texts[i-1] = new Image("locale/"+g_localestring+"/graphics/mainmenu/"+String::format("text%u.bmp", i));
    }

    return true;
}

void ScMainMenu::unload()
{
    for(uint8_t i = 0; i < 8; i++)
    {
        if(mImg_Buttons[i])
            delete mImg_Buttons[i];
        mImg_Buttons[i] = NULL;
        if(mImg_ButtonsP[i])
            delete mImg_ButtonsP[i];
        mImg_ButtonsP[i] = NULL;
        if(mImg_Texts[i])
            delete mImg_Texts[i];
        mImg_Texts[i] = NULL;
    }

    if(mImg_Mask) delete mImg_Mask;
    mImg_Mask = NULL;
    if(mImg_Background) delete mImg_Background;
    mImg_Background = NULL;
}

void ScMainMenu::display()
{
    if(getLoadedState() != Loaded)
    {
        R_SetMouse(mc_wait);
        return;
    }

    R_SetMouse(mc_select);

    int32_t pX = r_width / 2 - 320;
    int32_t pY = r_height / 2 - 240;

    int32_t mX = Events.getMouseX() - pX;
    int32_t mY = Events.getMouseY() - pY;

    uint32_t px = mImg_Mask->getPixelAt(mX, mY);

    uint32_t btnHovered = mButtonHovered;
    uint32_t btnClicked = mButtonClicked;

    if(px == Image::OutOfRange)
    {
        btnHovered = 0;
    }
    else
    {
        uint8_t ppx = ((px & 0xF0) >> 4)-7;
        if(ppx < 1 || ppx > 8) ppx = 0;
        btnHovered = ppx;
    }

    if((btnHovered != mButtonHovered) ||
       (btnClicked != mButtonClicked))
    {
        mChangedBg = true;
        mButtonHovered = btnHovered;
        mButtonClicked = btnClicked;
    }

    if(mChangedBg)
    {
        mImg_Background->display(pX, pY);
        int32_t bX = 0;
        int32_t bY = 0;
        uint8_t bNum = ((mButtonClicked) ? mButtonClicked : mButtonHovered);
        switch(bNum)
        {
            case 1:
                bX = 204;
                bY = 52;
                break;
            case 2:
                bX = 124;
                bY = 156;
                break;
            case 3:
                bX = 124;
                bY = 252;
                break;
            case 4:
                bX = 208;
                bY = 340;
                break;
            case 5:
                bX = 340;
                bY = 52;
                break;
            case 6:
                bX = 424;
                bY = 152;
                break;
            case 7:
                bX = 412;
                bY = 260;
                break;
            case 8:
                bX = 344;
                bY = 348;
                break;
            default:
                break;
        }

        if(bNum)
        {
            if(!mButtonClicked)
                mImg_Buttons[bNum-1]->display(pX+bX, pY+bY);
            else mImg_ButtonsP[bNum-1]->display(pX+bX, pY+bY);
        }

        R_UpdateRect(pX, pY, 640, 480);
        mChangedBg = false;
    }
}

void ScMainMenu::onEvent(const Event& e)
{
    if(e.type == Event::MouseEvent && e.isDown && e.button == 1)
    {
        mButtonClicked = mButtonHovered;
        if(mButtonClicked) mChangedBg = true;
    }
    else if(e.type == Event::MouseEvent && !e.isDown && e.button == 1)
    {
        if(mButtonClicked) mChangedBg = true;
        switch(mButtonClicked)
        {
            case 1: // new game
                break;
            case 2: // multiplayer
                break;
            case 3: // videos
                break;
            case 4: // authors
                break;
            case 5: // load game
                break;
            case 6: // hat
                break;
            case 7: // records
                break;
            case 8: // quit
                Events.quit();
                break;
            default:
                break;
        }
        mButtonClicked = 0;
    }
}
