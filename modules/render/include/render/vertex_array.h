#pragma once

#include "buffer.h"

namespace selyan
{
    class VertexArray
    {
    public:
        static VertexArray *create();

        virtual void
        setVertexBuffers(std::initializer_list<VertexBuffer *> buffers) = 0;

        virtual void setIndexBuffer(IndexBuffer *buffer) = 0;

        virtual void setInstanceCount(uint32_t primCount) = 0;

        virtual std::vector<VertexBuffer *> getVertexBuffers() const = 0;

        virtual IndexBuffer *getIndexBuffer() const = 0;

        virtual uint32_t getInstanceCount() const = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;
    };

}
