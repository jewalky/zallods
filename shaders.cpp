#include "shaders.hpp"
#include <fstream>
#include "core.hpp"
#include "display.hpp"

namespace shaders
{
    Shader::Shader(String vertex, String fragment)
    {
        context = 0;
        valid = false;
        program = 0;

        v_code = String::null();
        f_code = String::null();

        if(vertex.isNull() && fragment.isNull())
            return;

        if(!vertex.isNull())
        {
            std::ifstream vfile(vertex);
            String shaderCode = "";
            String shaderLine;
            while(getline(vfile, shaderLine))
                shaderCode += shaderLine.trim()+"\r\n";
            v_code = shaderCode;
        }

        if(!fragment.isNull())
        {
            std::ifstream ffile(fragment);
            String shaderCode = "";
            String shaderLine;
            while(getline(ffile, shaderLine))
                shaderCode += shaderLine.trim()+"\r\n";
            f_code = shaderCode;
        }

        compile();
    }

    bool Shader::activate()
    {
        if(!valid)
            return false;

        if(context != display::context_id())
        {
            program = 0;
            compile();
            if(!valid)
                return false;
        }

        glUseProgram(program);
        return true;
    }

    bool Shader::deactivate()
    {
        if(!valid)
            return false;
        GLint current_program = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        if(current_program == program)
            glUseProgram(0);
        return true;
    }

    bool Shader::compile()
    {
        if(program)
            return false;

        GLuint s_v = 0;
        GLuint s_f = 0;

        if(!v_code.isNull())
        {
            String shaderCode = v_code;

            s_v = glCreateShader(GL_VERTEX_SHADER);
            GLint len = shaderCode.length();
            const char* code = shaderCode.c_str();
            glShaderSourceARB(s_v, 1, &code, &len);
            glCompileShaderARB(s_v);

            GLint compiled;
            glGetObjectParameterivARB(s_v, GL_COMPILE_STATUS, &compiled);
            if(!compiled)
            {
                GLint blen = 0;
                GLsizei slen = 0;

                glGetShaderiv(s_v, GL_INFO_LOG_LENGTH , &blen);
                if(blen > 1)
                {
                    GLchar* compiler_log = (GLchar*)malloc(blen);
                    glGetInfoLogARB(s_v, blen, &slen, compiler_log);
                }

                glDeleteShader(s_v);

                return false;
            }
        }

        if(!f_code.isNull())
        {
            String shaderCode = f_code;

            s_f = glCreateShader(GL_FRAGMENT_SHADER);
            GLint len = shaderCode.length();
            const char* code = shaderCode.c_str();
            glShaderSourceARB(s_f, 1, &code, &len);
            glCompileShaderARB(s_f);

            GLint compiled;
            glGetObjectParameterivARB(s_f, GL_COMPILE_STATUS, &compiled);
            if(!compiled)
            {
                if(s_v) glDeleteShader(s_v);
                GLint blen = 0;
                GLsizei slen = 0;

                glGetShaderiv(s_f, GL_INFO_LOG_LENGTH , &blen);
                if(blen > 1)
                {
                    GLchar* compiler_log = (GLchar*)malloc(blen);
                    glGetInfoLogARB(s_f, blen, &slen, compiler_log);
                }

                glDeleteShader(s_f);
                return false;
            }
        }

        GLuint p = glCreateProgram();
        if(s_v) glAttachShader(p, s_v);
        if(s_f) glAttachShader(p, s_f);
        glLinkProgram(p);

        GLint linked;
        glGetProgramivARB(p, GL_LINK_STATUS, &linked);
        if(!linked)
        {
            if(s_v) glDeleteShader(s_v);
            if(s_f) glDeleteShader(s_f);
            glDeleteProgram(p);
            return false;
        }

        program = p;
        valid = true;
        context = display::context_id();

        return true;
    }

    Shader palette;

    bool init()
    {
        palette = Shader(String::null(), "data/shaders/palette/palette.fs");
        if(!palette.is_valid())
            return false;
        return true;
    }
}
