#include "OGLVertexArray.h"

#include <cassert>

namespace selyan
{
    VertexArray *VertexArray::create() { return new OGLVertexArray(); }

    OGLVertexArray::OGLVertexArray() : m_index(0), m_indexBuffer(nullptr), m_instanceCount(1)
    {
        glGenVertexArrays(1, &m_index);
    }

    void OGLVertexArray::setVertexBuffers(std::initializer_list<VertexBuffer *> buffers)
    {
        // RN_ASSERT(buffers.size() <= MAX_VERTEX_BUFFERS, "buffers.size() >
        // m_vertexBuffers.size()");
        assert(buffers.size() <= MAX_VERTEX_BUFFERS && "buffers.size() > m_vertexBuffers.size()");
        bind();
        GLuint index = 0;
        for (auto initBuffer : buffers)
        {
            if (initBuffer == nullptr)
                continue;

            m_vertexBuffers.push_back(initBuffer);

            VertexBuffer *buffer = m_vertexBuffers.back();
            buffer->bind();
            auto bufferLayout = buffer->getBufferLayout();
            GLuint stride = bufferLayout.getStride();
            for (auto const &element : bufferLayout)
            {
                GLenum type;
                GLboolean normalize = element.Normalize ? GL_TRUE : GL_FALSE;
                switch (element.Type)
                {
                    case float2:
                        type = GL_FLOAT;
                        break;
                    case float3:
                        type = GL_FLOAT;
                        break;
                    case none:
                        // RN_CORE_ERROR("SetVertexBuffer element no type");
                        std::cout << "SetVertexBuffer element no type" << std::endl;
                    default:
                        // RN_CORE_ERROR("From setVertexBuffers -> default in type switch");
                        std::cout << "From setVertexBuffers -> default in type switch" << std::endl;
                }
                glVertexAttribPointer(index,
                                      element.Size,
                                      type,
                                      normalize,
                                      stride,
                                      (void *)element.Offset);
                glEnableVertexAttribArray(index);
                glVertexAttribDivisor(index, GLuint(element.Divisor));
                index++;
            }
            buffer->unbind();
        }
        unbind();
    }

    void OGLVertexArray::setIndexBuffer(IndexBuffer *buffer)
    {
        m_indexBuffer = buffer;
        bind();
        m_indexBuffer->bind();
        unbind();
    }

    std::vector<VertexBuffer *> OGLVertexArray::getVertexBuffers() const { return m_vertexBuffers; }

    IndexBuffer *OGLVertexArray::getIndexBuffer() const { return m_indexBuffer; }

    void OGLVertexArray::bind() { glBindVertexArray(m_index); }

    void OGLVertexArray::unbind() { glBindVertexArray(0); }

}
