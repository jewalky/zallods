#include "String.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>

String String::format(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    int strl = vsnprintf(NULL, 0, format.c_str(), va);
    char* line = new char[strl+1];
    line[strl] = 0;
    vsnprintf(line, strl, format.c_str(), va);
    va_end(va);

    String s(line);
    delete[] line;

    return s;
}

StringList String::split(const String& separator)
{
    StringList sl;
    String s = "";

    size_t lastPos = find(separator);
    do
    {
        s = (*this);
        s.erase(lastPos+separator.length());
        sl.push_back(s);
        this->erase(0, lastPos+separator.length());
        lastPos = find(separator);
    }
    while(lastPos != npos);

    return sl;
}
