#pragma once

#include "render/vertex_array.h"

#include <glad/glad.h>

namespace selyan
{
    constexpr size_t MAX_VERTEX_BUFFERS = 3;

    class OGLVertexArray : public VertexArray
    {
    public:
        OGLVertexArray();

        void setVertexBuffers(std::initializer_list<VertexBuffer *> buffers) override;

        void setIndexBuffer(IndexBuffer *buffer) override;

        void setInstanceCount(uint32_t primCount) override { m_instanceCount = primCount; }

        std::vector<VertexBuffer *> getVertexBuffers() const override;

        IndexBuffer *getIndexBuffer() const override;

        uint32_t getInstanceCount() const override { return m_instanceCount; }

        void bind() override;

        void unbind() override;

    private:
        GLuint m_index;

        GLuint m_instanceCount;

        std::vector<VertexBuffer *> m_vertexBuffers;
        IndexBuffer *m_indexBuffer;

        void (OGLVertexArray::*m_drawFun)() const;
    };

}
