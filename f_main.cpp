#include "f_main.h"
#include "f_file.h"
#include "c_main.h"

bool F_AddResource(String file, String name)
{
    if(!Archives.open(file, name))
    {
        C_Fatal("F_Init: Resource \"%s\" not found.\n", file.c_str());
        return false;
    }

    C_Printf("F_Init:  Loaded \"%s\".\n", file.c_str());
    return true;
}

bool F_Init()
{
    C_Printf("F_Init: Loading resource files...\n");
    if(!F_AddResource("main.res", "main"))
        return false;
    if(!F_AddResource("graphics.res", "graphics"))
        return false;
    if(!F_AddResource("patch.res", "patch"))
        return false;
    if(!F_AddResource("world.res", "world"))
        return false;
    if(!F_AddResource("locale.res", "locale"))
        return false;
    if(!F_AddResource("sfx.res", "sfx"))
        return false;

    return true;
}
