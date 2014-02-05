#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include "String.hpp"
#include <iostream>

namespace core
{
    struct __printf_format
    {
        int precision;
        int width;
        String flags;
        char type;
    };

    inline void __printf(String& out, String format)
    {
        __printf_format nformat;

        while(format.length() > 0)
        {
            if(format[0] == '%')
            {
                nformat.flags = "";
                nformat.width = 0;
                nformat.precision = 6;
                nformat.type = 0;

                size_t sz = 1;
                // flags
                for(sz = 1; sz < format.size(); sz++)
                {
                    if(format[sz] == '\'')
                        nformat.flags += "'";
                    else if(format[sz] == '-')
                        nformat.flags += "-";
                    else if(format[sz] == '+')
                        nformat.flags += "+";
                    else if(format[sz] == ' ')
                        nformat.flags += " ";
                    else if(format[sz] == '#')
                        nformat.flags += "#";
                    else if(format[sz] == '0')
                        nformat.flags += "0";
                    else break;
                }

                String widthS = "";

                // width
                for(; sz < format.size(); sz++)
                {
                    if(format[sz] >= '0' && format[sz] <= '9')
                        widthS += format[sz];
                    else break;
                }

                if(!sscanf(widthS.c_str(), "%d", &nformat.width))
                    nformat.width = 0;

                // precision
                if(format[sz] == '.')
                {
                    widthS = "";
                    for(sz++; sz < format.size(); sz++)
                    {
                        if(format[sz] >= '0' && format[sz] <= '9')
                            widthS += format[sz];
                        else break;
                    }

                    if(!sscanf(widthS.c_str(), "%d", &nformat.precision))
                        nformat.precision = 6;
                }

                for(; sz < format.size(); sz++)
                {
                    // hLljZzt
                    if(format[sz] != 'h' &&
                       format[sz] != 'L' &&
                       format[sz] != 'l' &&
                       format[sz] != 'j' &&
                       format[sz] != 'Z' &&
                       format[sz] != 'z' &&
                       format[sz] != 't')
                        break;
                }

                if(format[sz] == '%')
                {
                    out += "%";
                    format.erase(0, sz+1);
                    continue;
                }
                else if(format[sz] == 'n')
                {
                    out += "\n";
                    format.erase(0, sz+1);
                    continue;
                }
                else
                {
                    out += "(null)";
                    format.erase(0, sz+1);
                    continue;
                }
            }
            else
            {
                out += format[0];
                format.erase(0, 1);
            }
        }
    }

    inline String __printf_expand(__printf_format f, void* p)
    {
        String formatStr = "%";
        formatStr += "0";
        formatStr += String::formatRaw("%d", sizeof(p)*2);
        formatStr += (f.type == 'p' || f.type == 'x') ? "x" : "X";
        return String::formatRaw(formatStr, p);
    }

    inline String __printf_expand(__printf_format f, float p)
    {
        if(f.type == 's' || f.type == 'f')
        {
            String formatStr = "%";
            formatStr += f.flags;
            if(f.width != 0)
                formatStr += String::formatRaw("%d", f.width);
            formatStr += String::formatRaw(".%d", f.precision);
            formatStr += "f";
            return String::formatRaw(formatStr, p);
        }
        return "0.0";
    }
    inline String __printf_expand(__printf_format f, double p) { return __printf_expand(f, (float)p); }

    inline String __printf_expand(__printf_format f, signed long long int p)
    {
        if(f.type == 's' || f.type == 'd' || f.type == 'f' || f.type == 'u')
        {
            String formatStr = "%";
            formatStr += f.flags;
            if(f.width != 0)
                formatStr += String::formatRaw("%d", f.width);
            if(f.type != 'f')
            {
                formatStr += "lld";
                return String::formatRaw(formatStr, p);
            }
            else
            {
                float ff = p;
                return __printf_expand(f, ff);
            }
        }
        else if(f.type == 'x' || f.type == 'X')
        {
            String formatStr = "%";
            formatStr += f.flags;
            if(f.width != 0)
                formatStr += String::formatRaw("%d", f.width);
            formatStr += "ll";
            formatStr += f.type;
            return String::formatRaw(formatStr, p);
        }
        else if(f.type == 'p' || f.type == 'P')
        {
            return __printf_expand(f, (void*)&p);
        }

        return "0";
    }
    inline String __printf_expand(__printf_format f, int p) { return __printf_expand(f, (signed long long int)p); }
    inline String __printf_expand(__printf_format f, long unsigned int p) { return __printf_expand(f, (signed long long int)p); }
    inline String __printf_expand(__printf_format f, unsigned int p) { return __printf_expand(f, (signed long long int)p); }
    inline String __printf_expand(__printf_format f, long int p) { return __printf_expand(f, (signed long long int)p); }

    inline String __printf_expand(__printf_format f, String p)
    {
        if(f.type == 's')
        {
            String formatStr = "%";
            formatStr += f.flags;
            if(f.width != 0)
                formatStr += String::formatRaw("%d", f.width);
            formatStr += "s";
            return String::formatRaw(formatStr, p.c_str());
        }
        else if(f.type == 'd' || f.type == 'u')
        {
            signed long long int slli = 0;
            if(!sscanf(p.c_str(), "%lld", &slli))
                slli = 0;
            return __printf_expand(f, slli);
        }
        else if(f.type == 'x' || f.type == 'X')
        {
            signed long long int slli = 0;
            String eFmt = "%ll";
            eFmt += f.type;
            if(!sscanf(p.c_str(), eFmt.c_str(), &slli))
                slli = 0;
            return __printf_expand(f, slli);
        }
        else if(f.type == 'f')
        {
            float sd = 0.0;
            if(!sscanf(p.c_str(), "%f", &sd))
                sd = 0.0;
            return __printf_expand(f, sd);
        }
        else if(f.type == 'p' || f.type == 'P')
        {
            return __printf_expand(f, (void*)&p);
        }

        return "(null)";
    }

    inline String __printf_expand(__printf_format f, const char* p)
    {
        if(f.type == 'p' || f.type == 'P') return __printf_expand(f, (void*)p);
        else if(!p) return __printf_expand(f, "(null)");
        else return __printf_expand(f, String(p));
    }

    template<typename H, typename... T>
    inline void __printf(String& out, String format, H p, T... t)
    {
        __printf_format nformat;

        while(format.length() > 0)
        {
            if(format[0] == '%')
            {
                nformat.flags = "";
                nformat.width = 0;
                nformat.precision = 6;
                nformat.type = 0;

                size_t sz = 1;
                // flags
                for(sz = 1; sz < format.size(); sz++)
                {
                    if(format[sz] == '\'')
                        nformat.flags += "'";
                    else if(format[sz] == '-')
                        nformat.flags += "-";
                    else if(format[sz] == '+')
                        nformat.flags += "+";
                    else if(format[sz] == ' ')
                        nformat.flags += " ";
                    else if(format[sz] == '#')
                        nformat.flags += "#";
                    else if(format[sz] == '0')
                        nformat.flags += "0";
                    else break;
                }

                String widthS = "";

                // width
                for(; sz < format.size(); sz++)
                {
                    if(format[sz] >= '0' && format[sz] <= '9')
                        widthS += format[sz];
                    else break;
                }

                if(!sscanf(widthS.c_str(), "%d", &nformat.width))
                    nformat.width = 0;

                // precision
                if(format[sz] == '.')
                {
                    widthS = "";
                    for(sz++; sz < format.size(); sz++)
                    {
                        if(format[sz] >= '0' && format[sz] <= '9')
                            widthS += format[sz];
                        else break;
                    }

                    if(!sscanf(widthS.c_str(), "%d", &nformat.precision))
                        nformat.precision = 6;
                }

                for(; sz < format.size(); sz++)
                {
                    // hLljZzt
                    if(format[sz] != 'h' &&
                       format[sz] != 'L' &&
                       format[sz] != 'l' &&
                       format[sz] != 'j' &&
                       format[sz] != 'Z' &&
                       format[sz] != 'z' &&
                       format[sz] != 't')
                        break;
                }

                if(format[sz] == '%')
                {
                    out += "%";
                    format.erase(0, sz+1);
                    continue;
                }
                else if(format[sz] == 'n')
                {
                    out += "\n";
                    format.erase(0, sz+1);
                    continue;
                }
                else if(format[sz] == 's' ||
                        format[sz] == 'd' ||
                        format[sz] == 'f' ||
                        format[sz] == 'x' ||
                        format[sz] == 'X' ||
                        format[sz] == 'p' ||
                        format[sz] == 'P' ||
                        format[sz] == 'u')
                {
                    nformat.type = format[sz];
                    out += __printf_expand(nformat, p);
                    format.erase(0, sz+1);
                    break;
                }
                else
                {
                    out += "(null)";
                    format.erase(0, sz+1);
                    continue;
                }
            }
            else
            {
                out += format[0];
                format.erase(0, 1);
            }
        }

        if(!format.length())
            __printf(out, format);
        else __printf(out, format, t...);
    }

    template<typename... T>
    inline void printf(const String& format, T... t)
    {
        String out = "";
        __printf(out, format, t...);
        std::cout.write(out.c_str(), out.length());
    }

    void init_args(int argc, char* argv[]);
    size_t argc();
    String arg(size_t index);
    String sub_arg(String arg, size_t index);
    bool has_arg(String arg);

    void abort();
}

#endif // CORE_HPP_INCLUDED
