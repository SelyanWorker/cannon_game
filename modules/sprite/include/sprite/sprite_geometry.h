#pragma once

#include "render/vertex_array.h"
#include "render/renderer.h"

#include <glm/glm.hpp>

namespace selyan
{
    struct Vertex2d
    {
        glm::vec3 position;
        glm::vec2 textureCoords;
        glm::vec2 normal;
    };

    class SpriteGeometry
    {
    public:
        SpriteGeometry(const glm::vec2 &geometrySize = { 1, 1 },
                       const glm::vec2 &textureSize = { 1, 1 },
                       float z = -1,
                       const glm::vec2 &offset = { 0, 0 })
        {
            const float halfWidth = geometrySize.x / 2.f ;
            const float halfHeight = geometrySize.y / 2.f;

            m_vertices[0] = { { -halfWidth + offset.x, -halfHeight + offset.y, z }, { 0, 0 }, { 0, 0 } };               // bottom left
            m_vertices[1] = { { -halfWidth + offset.x, halfHeight + offset.y, z }, { 0, textureSize.y }, { 0, 0 } };   // top left
            m_vertices[2] = { { halfWidth + offset.x, -halfHeight + offset.y, z }, { textureSize.x, 0 }, { 0, 0 } };   // bottom right
            m_vertices[3] = { { halfWidth + offset.x, halfHeight + offset.y, z }, textureSize, { 0, 0 } };            // top right

            m_vertexBuffer = VertexBuffer::create(sizeof(Vertex2d) * 4, m_vertices);
            m_vertexArray = VertexArray::create();

            auto elements = {
                BufferElement(float3, false),
                BufferElement(float2, false),
                BufferElement(float2, false),
            };
            m_vertexBuffer->setBufferLayout({ elements, 4 });

            m_vertexArray->setVertexBuffers({ m_vertexBuffer });
        }

        void draw() const
        {
            drawVertexArray(RenderMode::triangle_strip, m_vertexArray);
        }

    private:
        Vertex2d m_vertices[4];

        VertexBuffer *m_vertexBuffer;
        VertexArray *m_vertexArray;
    };
}
