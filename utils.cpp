#include <fstream>
#include "utils.h"

bool FileExists(const String& filename)
{
    std::ifstream f_temp;
    f_temp.open(filename.c_str(), std::ios::in | std::ios::binary);
    if(!f_temp.is_open()) return false;
    f_temp.close();
    return true;
}

String Basename(const String& filename)
{
    String ret = FixSlashes(filename);
    uint32_t where = ret.find_last_of('/');
    if(where == String::npos) return ret;
    ret.erase(0, where+1);
    return ret;
}

String FixSlashes(const String& filename)
{
    String ret = filename;
    for(String::iterator i = ret.begin(); i != ret.end(); ++i)
        if((*i) == '\\') (*i) = '/';
    return ret;
}

String TruncateSlashes(const String& filename)
{
    String ret = filename;
    char lastchar = 0;
    char thischar = 0;
    for(String::iterator i = ret.begin(); i != ret.end(); ++i)
    {
        thischar = (*i);
        if((thischar == '/' || thischar == '\\') &&
           (lastchar == '/' || lastchar == '\\'))
        {
            ret.erase(i);
            i--;
        }
        lastchar = thischar;
    }
    return ret;
}

bool Directory::Open(const String& what)
{
    this->Close();
    this->directory = opendir(what.c_str());
    if(!this->directory) return false;
    return true;
}

bool Directory::Read(DirectoryEntry& where)
{
    if(!this->directory) return false;
    struct dirent* dp = readdir(this->directory);
    if(!dp) return false;
    if(dp->d_name[0] == '.') return this->Read(where);
    where.name.assign(dp->d_name);
    return true;
}

void Directory::Close()
{
    if(this->directory) closedir(this->directory);
    this->directory = NULL;
}

Directory::~Directory()
{
    this->Close();
}

Directory::Directory()
{
    this->directory = NULL;
}
