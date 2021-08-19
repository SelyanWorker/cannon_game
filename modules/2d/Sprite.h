#pragma once

#include "SpriteSheet.h"
#include "core/core.h"
#include "geometry/vertex.h"
#include "math/math.h"
#include "render/shader.h"
#include "render/texture_2d.h"

#include "render/buffer.h"
#include "render/renderer.h"
#include "render/vertex_array.h"

namespace Rainy
{
    struct SpriteFrame
    {
        uint32_t row;
        uint32_t column;
        float lifeTime;
    };

    class RAINY_API Sprite
    {
    public:
        explicit Sprite(SpriteSheet *spriteSheet, SpriteFrame firstFrame = { 1, 1, 1 });

        ~Sprite();

        void AddSpriteFrame(uint32_t row, uint32_t column, float lifeTime);

        void Draw(Shader *program);

    private:
        Vertex2D m_vertices[4];

        VertexBuffer *m_vertexBuffer;
        VertexArray *m_vertexArray;

        Matrix3f m_textureMatrix;
        SpriteSheet *m_sheet;

        std::vector<SpriteFrame> m_frames;
        uint32_t m_currentFrame;
    };

}
