#include <SDL/SDL.h>
#include "String.h"
#include <cstdio>

int main(int argc, char* argv[])
{
    String s = "1 2 3 4";
    StringList sl = s.split(" ");
    for(size_t i = 0; i < sl.size(); i++)
        printf("%s\n", sl[i].c_str());
    return 0;
}
