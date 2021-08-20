#pragma once

#include "buffer.h"

namespace selyan
{
    class VertexArray
    {
    public:
        static VertexArray *Create();

        virtual void SetVertexBuffers(std::initializer_list<VertexBuffer *> buffers) = 0;

        virtual void SetIndexBuffer(IndexBuffer *buffer) = 0;

        virtual void SetInstanceCount(uint32_t primCount) = 0;

        virtual std::vector<VertexBuffer *> GetVertexBuffers() const = 0;

        virtual IndexBuffer *GetIndexBuffer() const = 0;

        virtual uint32_t GetInstanceCount() const = 0;

        virtual void Bind() = 0;

        virtual void UnBind() = 0;
    };

}
