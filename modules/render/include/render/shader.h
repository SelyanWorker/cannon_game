#pragma once

#include "buffer.h"

#include <glm/glm.hpp>

namespace selyan
{
    class Shader
    {
    public:
        static Shader *Create(std::vector<std::string> &src);

        virtual void Bind() = 0;

        virtual void UnBind() = 0;

        virtual void SetUniform(const char *name, bool value) = 0;

        virtual void SetUniform(const char *name, uint32_t value) = 0;

        virtual void SetUniform(const char *name, int32_t value) = 0;

        virtual void SetUniform(const char *name, float value) = 0;

        virtual void SetUniform(const char *name, const glm::vec2& vector) = 0;

        virtual void SetUniform(const char *name, const glm::vec3& vector) = 0;

        virtual void SetUniform(const char *name, const glm::vec4& vector) = 0;

        virtual void SetUniform(const char *name, const glm::mat3& matrix, bool transpose = true) = 0;

        virtual void SetUniform(const char *name, const glm::mat4& matrix, bool transpose = true) = 0;

        virtual glm::vec4 GetUniformValue(const char *name) const = 0;

        virtual void AddStorageBlock(ShaderBuffer *buffer, const char *name) = 0;
    };

}
