#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <string>
#include <vector>

class String;
typedef std::vector<String> StringList;

class String : std::string
{
    public:
        String(const char* c) : std::string(c) {}
        String(const std::string& s) : std::string(s) {}

        static String format(const String& format, ...);
        StringList split(const String& separator);
        void trim();

        const char* c_str() { return std::string::c_str(); }
};

#endif // STRING_H_INCLUDED
