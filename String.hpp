#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <string>
#include <vector>
#include <cstdarg>
#include <cstdio>

class String;
typedef std::vector<String> StringList;

namespace core
{
    template<typename H, typename... T>
    inline void __printf(String& out, String format, H p, T... t);
    template<typename... T>
    inline void printf(const String& format, T... t);
}

class String : public std::string
{
    public:
        void reset() { is_null = false; }

        String() : std::string() {reset();}
        String(const char* c) : std::string(c) {reset();}
        String(const char* c, size_t len) : std::string(c, len) {reset();}
        String(const String& s) : std::string(s) {reset();is_null=s.is_null;}
        String(const std::string& s) : std::string(s) {reset();}
        String(const std::string& s, size_t pos, size_t len = std::string::npos) : std::string(s, pos, len) {reset();};
        String(size_t len, char c) : std::string(len, c) {reset();}

        StringList split(const String& separator) const;
        String substring(size_t begin = 0, size_t end = -1) const;

        template<typename... T>
        static String format(const String& format, T... t)
        {
            String oo;
            core::__printf(oo, format, t...);
            return oo;
        }

        static const String& null()
        {
            static String s;
            s.is_null = true;
            return s;
        }

        static String formatRaw(const String& format, ...); // do NOT fucking use

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

        void replace(const String& what, const String& withWhat);

        bool isNull() { return is_null; }

    private:
        bool is_null;
};

#endif // STRING_H_INCLUDED
