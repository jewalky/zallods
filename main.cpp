#include <SDL/SDL.h>
#include "String.h"
#include "v_main.h"
#include "c_main.h"
#include "g_main.h"
#include "f_main.h"

StringList g_argv;

int _main()
{
    C_Init();

    if(!V_Init()) return 1;
    if(!F_Init()) return 1;

    G_MainLoop();

    V_Quit();

    return 0;
}

int main(int argc, char* argv[])
{
    for(int i = 0; i < argc; i++)
        g_argv.push_back(argv[i]);

    return _main();
}

void Abort()
{
    V_Quit();
    exit(1);
}
