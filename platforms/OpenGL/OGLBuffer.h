#pragma once

#include "render/buffer.h"

#include <glad/glad.h>

namespace selyan
{
    class OGLVertexBuffer : public VertexBuffer
    {
    public:
        OGLVertexBuffer(uint32_t size, void *data = nullptr);

        ~OGLVertexBuffer();

        void setBufferLayout(BufferLayout layout) override;

        BufferLayout const &getBufferLayout() const override;

        void subData(int64_t offset, int64_t size, void *data) override;

        void reallocate(uint32_t size, void *data) override;

        void bind() override;

        void unbind() override;

    private:
        GLsizeiptr m_size;
        GLuint m_index;

        BufferLayout m_layout;
    };

    class OGLIndexBuffer : public IndexBuffer
    {
    public:
        OGLIndexBuffer(uint32_t size, uint32_t count, void *data = nullptr);

        ~OGLIndexBuffer();

        uint32_t getCount() const override;

        void subData(int64_t offset, int64_t size, void *data) override;

        void reallocate(uint32_t size, void *data, uint32_t count = 0) override;

        void bind() override;

        void unbind() override;

    private:
        GLsizeiptr m_size;
        GLsizei m_count;
        GLuint m_index;
    };

    class OGLShaderBuffer : public ShaderBuffer
    {
    public:
        OGLShaderBuffer(uint32_t size, void *data = nullptr);

        ~OGLShaderBuffer();

        void *getData(uint32_t offset, uint32_t size) const override;

        void subData(int64_t offset, int64_t size, void *data) override;

        void reallocate(uint32_t size, void *data) override;

        void bind() override;

        void unbind() override;

    private:
        GLsizeiptr m_size;
        GLsizei m_count;
        GLuint m_index;
    };

}
