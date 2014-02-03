#include "String.hpp"
#include "core.hpp"
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

String String::formatRaw(const String& format, ...)
{
    va_list va;
    va_start(va, format);
    int strl = vsnprintf(NULL, 0, format.c_str(), va)+1;
    va_end(va);
    va_start(va, format);
    char* line = new char[strl+1];
    line[strl] = 0;
    vsnprintf(line, strl, format.c_str(), va);
    va_end(va);

    String s(line);
    delete[] line;

    return s;
}

StringList String::split(const String& separator) const
{
    StringList sl;
    String s = "";
    String ms = (*this);

    size_t lastPos = ms.find(separator);
    do
    {
        s = ms;
        s.erase(lastPos);
        sl.push_back(s);
        ms.erase(0, lastPos+separator.length());
        lastPos = ms.find(separator);
    }
    while(lastPos != npos);

    sl.push_back(ms);

    return sl;
}

String String::substring(size_t begin, size_t end) const
{
    String r = (*this);
    if(end >= r.size())
        end = -1;
    if(end < 0) end = r.size();
    if(begin < 0) begin = 0;
    size_t len = end - begin;
    if(len < 0) return "";

    if(end < r.size()) r.erase(end);
    if(end > 0) r.erase(0, begin);

    return r;
}

bool String::isInteger() const
{
    for(size_t i = 0; i < length(); i++)
    {
        char c = (*this)[i];
        if((c > '9' || c < '0') && !(c == '-' && i == 0))
            return false;
    }

    return true;
}

int String::toInteger() const
{
    if(!isInteger()) return 0;
    return atoi(c_str());
}

bool String::isBoolean() const
{
    if(isInteger()) return true;
    String bs = toLowerCase();
    if(bs == "true" || bs == "false" ||
       bs == "yes" || bs == "no")
       return true;
    return false;
}

bool String::toBoolean() const
{
    if(isInteger()) return (toInteger());
    if(!isBoolean()) return false;
    String bs = toLowerCase();
    if(bs == "true" || bs == "yes")
        return true;
    return false;
}

String String::toLowerCase() const
{
    String s;
    for(size_t i = 0; i < length(); i++)
    {
        char c = (*this)[i];
        s.push_back(tolower(c));
    }

    return s;
}

bool String::isDouble() const
{
    for(size_t i = 0; i < length(); i++)
    {
        char c = (*this)[i];
        if((c > '9' || c < '0') && !(c == '-' && i == 0) && c != '.')
            return false;
    }

    return true;
}

float String::toDouble() const
{
    if(!isDouble()) return 0.0;
    return atof(c_str());
}

String String::toUpperCase() const
{
    String s;
    for(size_t i = 0; i < length(); i++)
    {
        char c = (*this)[i];
        s.push_back(toupper(c));
    }

    return s;
}

bool String::isWhitespace(char what)
{
    switch((unsigned char)what)
    {
        case ' ':
        case '\r':
        case '\n':
        case '\t':
        case 0xFF:
            return true;
        default:
            return false;
    }
}

String String::trim(bool (callback)(char))
{
    return trimLeft().trimRight();
}

String String::trimLeft(bool (callback)(char))
{
    String ret = (*this);
    for(String::iterator i = ret.begin(); i != ret.end(); ++i)
    {
        if(callback((*i)))
            continue;
        ret.erase(ret.begin(), i);
        return ret;
    }
    return "";
}

String String::trimRight(bool (callback)(char))
{
    String ret = (*this);
    for(String::reverse_iterator i = ret.rbegin(); i != ret.rend(); ++i)
    {
        if(callback((*i)))
            continue;
        ret.erase(i.base(), ret.end());
        return ret;
    }
    return "";
}

void String::replace(const String& what, const String& withWhat)
{
    size_t where;
    while((where = find(what)) != npos)
        std::string::replace(where, what.length(), withWhat);
}
