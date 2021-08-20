#pragma once

#include "render/shader.h"

#include <glad/glad.h>

namespace selyan
{
    class OGLShader : public Shader
    {
    public:
        OGLShader(std::vector<std::string> &src);

        ~OGLShader();

        void Bind() override;

        void UnBind() override;

        void SetUniform(const char *name, bool value) override;

        void SetUniform(const char *name, uint32_t value) override;

        void SetUniform(const char *name, int32_t value) override;

        void SetUniform(const char *name, float value) override;

        void SetUniform(const char *name, const glm::vec2& vector) override;

        void SetUniform(const char *name, const glm::vec3& vector) override;

        void SetUniform(const char *name, const glm::vec4& vector) override;

        void SetUniform(const char *name, const glm::mat3& matrix, bool transpose = true) override;

        void SetUniform(const char *name, const glm::mat4& matrix, bool transpose = true) override;

        glm::vec4 GetUniformValue(const char *name) const override;

        void AddStorageBlock(ShaderBuffer *buffer, const char *name) override;

        bool GetLinkStatus() const;

    private:
        GLint GetUniformLocation(const char *name) const;

        GLuint CreateShader(GLenum Type, const char *src, GLsizei srcLenght);

    private:
        GLuint m_index;
        GLuint m_vsIndex;
        GLuint m_fsIndex;
        GLuint m_gsIndex;
        GLuint m_tcsIndex;
        GLuint m_tesIndex;
        GLuint m_lastStorageIndex;
    };

}
