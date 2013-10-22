#include "c_main.h"
#include "main.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#ifdef __MINGW32__
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

// This file, among other stuff, contains lame crutch to suppress SDL's output redirection

FILE* g_stderr = NULL;
FILE* g_stdout = NULL;

void C_Init()
{
#ifndef __MINGW32__
    stderr = fopen("/dev/null", "wb");
    stdout = fopen("/dev/null", "wb");
#else
    if(!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        g_stderr = NULL;
        g_stdout = NULL;
    }
    else
    {
        freopen("CONOUT$", "wb", stderr);
        freopen("CONOUT$", "wb", stdout);
        g_stderr = stderr;
        g_stdout = stdout;
    }
#endif
}

void _C_FPrintf(FILE* f, const String& format, va_list va)
{
    if(!f) return;
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
