#ifndef G_EVENT_H_INCLUDED
#define G_EVENT_H_INCLUDED

#include <list>
#include <SDL/SDL.h>

struct Event
{
    enum
    {
        NoEvent,
        Quit,
        KeyboardEvent,
        TextEvent,
        MouseEvent,
    };

    int type;
    char chr;
    char sym;
    bool isDown;
    int button;

    Event()
    {
        type = NoEvent;
        chr = 0;
        sym = 0;
        isDown = false;
        button = 0;
    }
};

class Events
{
    public:
        Events();
        void eatEvent(const SDL_Event& e);
        void tick();

        bool getEvent(Event& ev);

        uint16_t getMouseX();
        uint16_t getMouseY();

        uint16_t getMouseMask();

        enum
        {
            ButtonLeft      = 0x01,
            ButtonMiddle    = 0x04,
            ButtonRight     = 0x02
        };

    private:
        std::list<Event> mEvents;

        bool mRepeatingDown;
        bool mRepeating;
        uint32_t mLastTicks;

        Event mRepeatingE;

        uint16_t mMouseX;
        uint16_t mMouseY;

        uint16_t mMouseMask;
};

extern class Events Events;

#endif // G_EVENT_H_INCLUDED
