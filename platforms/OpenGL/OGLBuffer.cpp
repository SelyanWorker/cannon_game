#include "OGLBuffer.h"

namespace selyan
{
    // vertex buffer
    VertexBuffer *VertexBuffer::create(uint32_t size, void *data)
    {
        return new OGLVertexBuffer(size, data);
    }

    OGLVertexBuffer::OGLVertexBuffer(uint32_t size, void *data) : m_size(size)
    {
        glGenBuffers(1, &m_index);
        glBindBuffer(GL_ARRAY_BUFFER, m_index);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OGLVertexBuffer::~OGLVertexBuffer() { glDeleteBuffers(1, &m_index); }

    void OGLVertexBuffer::setBufferLayout(BufferLayout layout) { m_layout = layout; }

    BufferLayout const &OGLVertexBuffer::getBufferLayout() const { return m_layout; }

    void OGLVertexBuffer::subData(int64_t offset, int64_t size, void *data)
    {
      bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        unbind();
    }

    void OGLVertexBuffer::reallocate(uint32_t size, void *data)
    {
      bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_size = size;
        unbind();
    }

    void OGLVertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, m_index); }

    void OGLVertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    // index buffer
    IndexBuffer *IndexBuffer::create(uint32_t size, uint32_t count, void *data)
    {
        return new OGLIndexBuffer(size, count, data);
    }

    OGLIndexBuffer::OGLIndexBuffer(uint32_t size, uint32_t count, void *data)
      : m_size(size),
        m_count(count)
    {
        glGenBuffers(1, &m_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OGLIndexBuffer::~OGLIndexBuffer() { glDeleteBuffers(1, &m_index); }

    uint32_t OGLIndexBuffer::getCount() const { return m_count; }

    void OGLIndexBuffer::subData(int64_t offset, int64_t size, void *data)
    {
      bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        unbind();
    }

    void OGLIndexBuffer::reallocate(uint32_t size, void *data, uint32_t count)
    {
      bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_ELEMENT_ARRAY_BUFFER);
        if (count != 0)
            m_count = count;
        unbind();
    }

    void OGLIndexBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index); }

    void OGLIndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    // shader buffer
    ShaderBuffer *ShaderBuffer::create(uint32_t size, void *data)
    {
        return new OGLShaderBuffer(size, data);
    }

    OGLShaderBuffer::OGLShaderBuffer(uint32_t size, void *data) : m_size(size)
    {
        glGenBuffers(1, &m_index);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_index);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    OGLShaderBuffer::~OGLShaderBuffer() { glDeleteBuffers(1, &m_index); }

    void *OGLShaderBuffer::getData(uint32_t offset, uint32_t size) const
    {
        uint8_t *data = new uint8_t[size];
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
        return data;
    }

    void OGLShaderBuffer::subData(int64_t offset, int64_t size, void *data)
    {
      bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
        unbind();
    }

    void OGLShaderBuffer::reallocate(uint32_t size, void *data)
    {
      bind();
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
        m_size = size;
        unbind();
    }

    void OGLShaderBuffer::bind()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_index);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_index);
    }

    void OGLShaderBuffer::unbind() { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); }

}
