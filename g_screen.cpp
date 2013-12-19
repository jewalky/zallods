#include "g_screen.h"
#include "c_main.h"

GenericScreen::GenericScreen()
{
    mThread = NULL;
    mLoadedState = NotSet;
    mDiscard = false;
}

GenericScreen::~GenericScreen()
{

}

int GenericScreen::runThread(void* v)
{
    GenericScreen* scr = (GenericScreen*)v;
    if(scr->load()) scr->mLoadedState = Loaded;
    else scr->mLoadedState = Failed;
    if(scr->mDiscard) scr->unload();
    return 0;
}

void GenericScreen::run()
{
    if(mLoadedState != NotSet) return;
    mThread = SDL_CreateThread(&runThread, this);
}

void GenericScreen::display()
{

}

void GenericScreen::onEvent(const Event& e)
{

}

bool GenericScreen::load()
{
    return true;
}

void GenericScreen::unload()
{
    delete this;
}

GenericScreen::LoadedState GenericScreen::getLoadedState()
{
    return mLoadedState;
}

GenericScreen* g_screen = NULL;

void G_SetScreen(GenericScreen* s)
{
    if(g_screen)
    {
        if(g_screen->getLoadedState() == GenericScreen::NotSet)
            g_screen->mDiscard = true;
        else g_screen->unload();

        g_screen = NULL;
    }

    g_screen = s;
    if(g_screen) g_screen->run();
}
