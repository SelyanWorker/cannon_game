#pragma once

#include "shader.h"
#include <unordered_map>

namespace selyan
{
    class ShaderLibrary
    {
    public:
        static Shader *createShader(std::istream *input, std::string shaderName = "");

        static Shader *createShaderFromFile(std::string file);

        static Shader *getShader(std::string name);

    private:
        static std::unordered_map<std::string, Shader *> m_shaders;
    };

}
