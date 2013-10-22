#ifndef C_MAIN_H_INCLUDED
#define C_MAIN_H_INCLUDED

#include "String.h"
void C_Init();
void C_Printf(const String& format, ...);
void C_Warning(const String& format, ...);
void C_Error(const String& format, ...);
void C_Fatal(const String& format, ...);

#endif // C_MAIN_H_INCLUDED
