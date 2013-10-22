#include <SDL/SDL.h>
#include "String.h"
#include "v_main.h"
#include "c_main.h"
#include "g_main.h"
#include "f_main.h"

StringList g_argv;

int main(int argc, char* argv[])
{
    C_Init();

    for(int i = 0; i < argc; i++)
        g_argv.push_back(argv[i]);

    if(!V_Init()) return 1;
    if(!F_Init()) return 1;

    G_MainLoop();

    V_Quit();
    return 0;
}

void Abort()
{
    V_Quit();
    exit(1);
}
