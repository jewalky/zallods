#include "c_main.h"
#include "main.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

// This file, among other stuff, contains lame crutch to suppress SDL's output redirection

FILE* g_stderr = NULL;
FILE* g_stdout = NULL;

void C_Init()
{
    g_stderr = stderr;
    g_stdout = stdout;
#ifndef __MINGW32__
    stderr = fopen("/dev/null", "wb");
    stdout = fopen("/dev/null", "wb");
#else
    stderr = fopen("NUL", "wb");
    stdout = fopen("NUL", "wb");
#endif
}

void _C_FPrintf(FILE* f, const String& format, va_list va)
{
    vfprintf(f, format.c_str(), va);
}

void C_Printf(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    _C_FPrintf(g_stdout, format, va);
    va_end(va);
}

void C_Warning(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    _C_FPrintf(g_stderr, "Warning: "+format, va);
    va_end(va);
}

void C_Error(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    _C_FPrintf(g_stderr, "Error: "+format, va);
    va_end(va);
}

void C_Fatal(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    _C_FPrintf(g_stderr, "Fatal: "+format, va);
    va_end(va);

    Abort();
}
