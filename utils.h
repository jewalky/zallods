#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <dirent.h>
#include <stdint.h>
#include "String.h"

// file related
bool FileExists(const String& file);
String Basename(const String& file);
String FixSlashes(const String& filename);
String TruncateSlashes(const String& filename);

// directory search
struct DirectoryEntry
{
    String name;
};

class Directory
{
public:
    Directory();
    ~Directory();

    bool Open(const String& what);
    bool Read(DirectoryEntry& where);
    void Close();

private:
    DIR* directory;
};

#endif // UTILS_H_INCLUDED
