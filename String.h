#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <string>
#include <vector>

class String;
typedef std::vector<String> StringList;

class String : public std::string
{
    public:
        String() : std::string() {}
        String(const char* c) : std::string(c) {}
        String(const std::string& s) : std::string(s) {}

        static String format(const String& format, ...);
        StringList split(const String& separator) const;
        String substring(size_t begin = 0, size_t end = -1) const;

        bool isInteger() const;
        int toInteger() const;

        bool isBoolean() const;
        bool toBoolean() const;

        bool isDouble() const;
        float toDouble() const;

        String toLowerCase() const;
        String toUpperCase() const;

        static bool isWhitespace(char what);
        String trim(bool (callback)(char) = &isWhitespace);
        String trimLeft(bool (callback)(char) = &isWhitespace);
        String trimRight(bool (callback)(char) = &isWhitespace);
};

#endif // STRING_H_INCLUDED
