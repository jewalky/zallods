#include "core.hpp"

namespace core
{
    std::vector<String> argv;

    void init_args(int argc, char* argv[])
    {
        for(int i = 1; i < argc; i++)
        {
            String arg = String(argv[i]);
            core::argv.push_back(arg);
        }
    }

    size_t argc()
    {
        return argv.size();
    }

    String sub_arg(String arg, size_t index)
    {
        arg = arg.toLowerCase();
        for(size_t i = 0; i < argv.size(); i++)
        {
            if(argv[i].toLowerCase() == arg)
            {
                if(i+1+index < argv.size())
                    return argv[i+1+index];
                else return "";
            }
        }

        return "";
    }

    bool has_arg(String arg)
    {
        arg = arg.toLowerCase();
        for(size_t i = 0; i < argv.size(); i++)
        {
            if(argv[i].toLowerCase() == arg)
                return true;
        }

        return false;
    }

    String arg(size_t index)
    {
        if(index >= argc())
            return String::null();
        return argv[index];
    }
}
