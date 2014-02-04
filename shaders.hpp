#ifndef SHADERS_HPP_INCLUDED
#define SHADERS_HPP_INCLUDED

#include "sys_graphics.hpp"
#include "String.hpp"
#include <cstdint>

namespace shaders
{
    class Shader
    {
        public:
            Shader(String vertex = String::null(), String fragment = String::null());

            bool compile();

            bool is_valid() { return valid; }

            bool activate();
            bool deactivate();

        private:
            bool valid;
            GLint program;

            String v_code;
            String f_code;

            uint32_t context;
    };

    extern Shader palette;

    bool init();
}

#endif // SHADERS_HPP_INCLUDED
