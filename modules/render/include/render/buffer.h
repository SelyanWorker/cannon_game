#pragma once

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <vector>

namespace selyan
{
    enum ShaderDataType : int32_t
    {
        none = 0,
        float2 = 1,
        float3 = 2
    };

    struct BufferElement
    {
        BufferElement(ShaderDataType type, bool normalize = false, uint32_t divisor = 0)
          : Type(type),
            Normalize(normalize),
            Divisor(divisor)
        {
        }

        ShaderDataType Type = ShaderDataType::none;
        bool Normalize = false;
        uint32_t Divisor = 0;   //  rate at which generic vertex attributes advance during instanced
                                //  rendering (from OGL doc)
        uint32_t Size = 1;
        uint32_t Offset = 0;
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> &elements, uint64_t vertexCount)
          : m_elements(elements),
            m_stride(0),
            m_vertexCount(vertexCount)
        {
            for (auto &element : m_elements)
            {
                switch (element.Type)
                {
                    case float2:
                        element.Size = 2;
                        element.Offset = m_stride;
                        m_stride += sizeof(float) * 2;
                        break;
                    case float3:
                        element.Size = 3;
                        element.Offset = m_stride;
                        m_stride += sizeof(float) * 3;
                        break;
                    case none:
                        std::cout << "No data type specified!" << std::endl;
                        break;
                    default:
                        std::cout << "From BufferLayout constructor -> default in type switch"
                                  << std::endl;
                }
            }
        }

        uint32_t getStride() const { return m_stride; }
        uint64_t getVertexCount() const { return m_vertexCount; }

        std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }

    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_stride = 0;
        uint64_t m_vertexCount = 0;
    };

    class VertexBuffer
    {
    public:
        static VertexBuffer *create(uint32_t size, void *data);

        virtual void setBufferLayout(BufferLayout layout) = 0;

        virtual BufferLayout const &getBufferLayout() const = 0;

        virtual void subData(int64_t offset, int64_t size, void *data) = 0;

        virtual void reallocate(uint32_t size, void *data) = 0;
        // virtual void* getData(uint32_t buffer, int64_t offset, int64_t size, void* data);

        virtual void bind() = 0;

        virtual void unbind() = 0;
    };

    class IndexBuffer
    {
    public:
        static IndexBuffer *create(uint32_t size, uint32_t count, void *data);

        virtual uint32_t getCount() const = 0;

        virtual void subData(int64_t offset, int64_t size, void *data) = 0;

        virtual void reallocate(uint32_t size, void *data, uint32_t count) = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;
    };

    class ShaderBuffer
    {
    public:
        static ShaderBuffer *create(uint32_t size, void *data);

        virtual void *getData(uint32_t offset, uint32_t size) const = 0;

        virtual void subData(int64_t offset, int64_t size, void *data) = 0;

        virtual void reallocate(uint32_t size, void *data) = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;
    };

}
