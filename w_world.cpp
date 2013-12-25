#include "w_world.h"
#include "f_file.h"
#include "c_main.h"

struct World World;

void W_Init()
{
    C_Printf("W_Init: initializing world data...\n");

    World.initialize();
}

void World::initialize()
{
    File f;
    if(!f.open("world/data/data.bin"))
        C_Fatal("World::initialize: Couldn't open \"world/data/data/bin\".\n");

    f.seek(0);
    // oops don't have any code at this point yet

    f.close();
}
