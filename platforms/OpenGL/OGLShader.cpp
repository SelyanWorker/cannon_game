
#include "OGLShader.h"
#include "render/shader.h"

namespace selyan
{
#define GET_UNIFORM_LOCATION(name, action)                                                         \
    GLint location = glGetUniformLocation(m_index, name);                                          \
    if (location < 0)                                                                              \
    /*std::cout << "uniform " <<  name << " not found in shader program" << std::endl*/;                \
    else                                                                                           \
        action

    Shader *Shader::create(std::vector<std::string> &src)
    {
        OGLShader *oglShader = new OGLShader(src);
        if (!oglShader->GetLinkStatus())
            return nullptr;
        return oglShader;
    }

    OGLShader::OGLShader(std::vector<std::string> &src)
      : m_index(GL_INVALID_INDEX),
        m_vsIndex(GL_INVALID_INDEX),
        m_fsIndex(GL_INVALID_INDEX),
        m_gsIndex(GL_INVALID_INDEX),
        m_tcsIndex(GL_INVALID_INDEX),
        m_tesIndex(GL_INVALID_INDEX),
        m_lastStorageIndex(GL_INVALID_INDEX)
    {
        m_lastStorageIndex = 0;
        if (src.size() > 5)
            // RN_APP_WARN("Rainy supported only 5 Types of the shaders");
            std::cout << "Rainy supported only 5 Types of the shaders" << std::endl;

        m_index = glCreateProgram();

        for (std::string &part : src)
        {
            uint32_t secondLineStart = part.find('#', 6);
            std::string shaderType = part.substr(6, secondLineStart - 7);
            if (shaderType == "vertex")
                m_vsIndex = CreateShader(GL_VERTEX_SHADER,
                                         part.data() + secondLineStart,
                                         part.size() - secondLineStart);
            else if (shaderType == "fragment")
                m_fsIndex = CreateShader(GL_FRAGMENT_SHADER,
                                         part.data() + secondLineStart,
                                         part.size() - secondLineStart);
            else if (shaderType == "geometry")
                m_gsIndex = CreateShader(GL_GEOMETRY_SHADER,
                                         part.data() + secondLineStart,
                                         part.size() - secondLineStart);
            else if (shaderType == "tesscontrol")
                m_tcsIndex = CreateShader(GL_TESS_CONTROL_SHADER,
                                          part.data() + secondLineStart,
                                          part.size() - secondLineStart);
            else if (shaderType == "tessevaluation")
                m_tesIndex = CreateShader(GL_TESS_EVALUATION_SHADER,
                                          part.data() + secondLineStart,
                                          part.size() - secondLineStart);
            else
                // RN_APP_WARN("Unsupported shader Type");
                std::cout << "Unsupported shader Type" << std::endl;
        }

        if (m_vsIndex != GL_INVALID_INDEX)
            glAttachShader(m_index, m_vsIndex);
        if (m_fsIndex != GL_INVALID_INDEX)
            glAttachShader(m_index, m_fsIndex);
        if (m_gsIndex != GL_INVALID_INDEX)
            glAttachShader(m_index, m_gsIndex);
        if (m_tcsIndex != GL_INVALID_INDEX)
            glAttachShader(m_index, m_tcsIndex);
        if (m_tesIndex != GL_INVALID_INDEX)
            glAttachShader(m_index, m_tesIndex);

        glLinkProgram(m_index);
        GLint programLinked;
        glGetProgramiv(m_index, GL_LINK_STATUS, &programLinked);
        if (programLinked != GL_TRUE)
        {
            GLsizei logLength = 0;
            GLchar message[1024];
            glGetProgramInfoLog(m_index, 1024, &logLength, message);
        }

        GLint answer[2];
        glGetProgramiv(m_index, GL_ACTIVE_ATTRIBUTES, &answer[0]);
        glGetProgramiv(m_index, GL_ACTIVE_UNIFORMS, &answer[1]);
        //        RN_CORE_INFO("Program index: {0}. Attibutes: {1}. Uniforms: {2}",
        //                     m_index,
        //                     answer[0],
        //                     answer[1]);
        std::cout << "Program index: " << m_index << " Attibutes: " << answer[0]
                  << "Uniforms: " << answer[1] << std::endl;
    }

    OGLShader::~OGLShader()
    {
        glDeleteShader(m_vsIndex);
        glDeleteShader(m_fsIndex);
        glDeleteProgram(m_index);
    }

    void OGLShader::bind()
    {
        glUseProgram(m_index);
        if (m_tcsIndex != GL_INVALID_INDEX)
        {
            GLint param;
            glGetProgramiv(m_index, GL_TESS_CONTROL_OUTPUT_VERTICES, &param);
            glPatchParameteri(GL_PATCH_VERTICES, param);
        }
    }

    void OGLShader::unbind() {}

    bool OGLShader::GetLinkStatus() const
    {
        GLint programLinked;
        glGetProgramiv(m_index, GL_LINK_STATUS, &programLinked);
        if (programLinked == GL_TRUE)
            return true;
        else
            return false;
    }

    void OGLShader::setUniform(const char *name, bool value)
    {
        GET_UNIFORM_LOCATION(name, { glUniform1i(location, GLint(value)); })
    }

    void OGLShader::setUniform(const char *name, uint32_t value)
    {
        GET_UNIFORM_LOCATION(name, { glUniform1ui(location, value); });
    }

    void OGLShader::setUniform(const char *name, int32_t value)
    {
        GET_UNIFORM_LOCATION(name, { glUniform1i(location, value); });
    }

    void OGLShader::setUniform(const char *name, float value)
    {
        GET_UNIFORM_LOCATION(name, { glUniform1f(GetUniformLocation(name), value); });
    }

    void OGLShader::setUniform(const char *name, const glm::vec2 &vector)
    {
        GET_UNIFORM_LOCATION(name, { glUniform2f(GetUniformLocation(name), vector.x, vector.y); });
    }

    void OGLShader::setUniform(const char *name, const glm::vec3 &vector)
    {
        GET_UNIFORM_LOCATION(name, {
            glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
        });
    }

    void OGLShader::setUniform(const char *name, const glm::vec4 &vector)
    {
        GET_UNIFORM_LOCATION(name, {
            glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
        });
    }

    void OGLShader::setUniform(const char *name, const glm::mat3 &matrix, bool transpose)
    {
        GET_UNIFORM_LOCATION(name, {
            glUniformMatrix3fv(GetUniformLocation(name),
                               1,
                               transpose ? GL_TRUE : GL_FALSE,
                               &matrix[0][0]);
        });
    }

    void OGLShader::setUniform(const char *name, const glm::mat4 &matrix, bool transpose)
    {
        GET_UNIFORM_LOCATION(name, {
            glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, &matrix[0][0]);
        });
    }

    glm::vec4 OGLShader::getUniformValue(const char *name) const
    {
        glm::vec4 val;
        GET_UNIFORM_LOCATION(name, {
            glGetUniformfv(m_index, location, reinterpret_cast<float *>(&val));
        });
        return val;
    }

    void OGLShader::addStorageBlock(ShaderBuffer *buffer, const char *name)
    {
        buffer->bind();
        GLuint index = glGetProgramResourceIndex(m_index, GL_SHADER_STORAGE_BLOCK, name);
        // RN_ASSERT(index != GL_INVALID_INDEX, "getUniformValue: uniform with this name not
        // found");
        assert(index != GL_INVALID_INDEX && "getUniformValue: uniform with this name not found");
        glShaderStorageBlockBinding(m_index, index, 2);
    }

    inline GLint OGLShader::GetUniformLocation(const char *name) const
    {
        GLint location = glGetUniformLocation(m_index, name);
        if (location < 0)
            // RN_CORE_INFO("uniform {0} not found in shader program", name);
            std::cout << "uniform" << name << "not found in shader program" << std::endl;
        return location;
    }

    GLuint OGLShader::CreateShader(GLenum Type, const char *src, GLsizei srcLenght)
    {
        GLuint shaderIndex = glCreateShader(Type);
        glShaderSource(shaderIndex, 1, &src, &srcLenght);
        glCompileShader(shaderIndex);

        GLint compileStatus;
        glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE)
        {
            GLsizei logLenght = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shaderIndex, 1024, &logLenght, message);
            switch (Type)
            {
                case GL_VERTEX_SHADER:
                    // RN_APP_INFO("Error in vertex shader source: {0}", message);
                    std::cout << "Error in vertex shader source: " << message << std::endl;
                    break;
                case GL_FRAGMENT_SHADER:
                    // RN_APP_INFO("Error in fragment shader source: {0}", message);
                    std::cout << "Error in fragment shader source: " << message << std::endl;
                    break;
                case GL_GEOMETRY_SHADER:
                    // RN_APP_INFO("Error in geometry shader source: {0}", message);
                    std::cout << "Error in geometry shader source: " << message << std::endl;
                    break;
            }
            glDeleteShader(shaderIndex);
            return GL_INVALID_INDEX;
        }
        return shaderIndex;
    }

}
