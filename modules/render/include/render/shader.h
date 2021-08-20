#pragma once

#include "buffer.h"

#include <glm/glm.hpp>

namespace selyan
{
    class Shader
    {
    public:
        static Shader *create(std::vector<std::string> &src);

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setUniform(const char *name, bool value) = 0;

        virtual void setUniform(const char *name, uint32_t value) = 0;

        virtual void setUniform(const char *name, int32_t value) = 0;

        virtual void setUniform(const char *name, float value) = 0;

        virtual void setUniform(const char *name, const glm::vec2& vector) = 0;

        virtual void setUniform(const char *name, const glm::vec3& vector) = 0;

        virtual void setUniform(const char *name, const glm::vec4& vector) = 0;

        virtual void setUniform(const char *name, const glm::mat3& matrix, bool transpose = true) = 0;

        virtual void setUniform(const char *name, const glm::mat4& matrix, bool transpose = true) = 0;

        virtual glm::vec4 getUniformValue(const char *name) const = 0;

        virtual void addStorageBlock(ShaderBuffer *buffer, const char *name) = 0;
    };

}
