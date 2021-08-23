#pragma once

#include "render/shader.h"
#include "render/texture_2d.h"
#include "sprite_sheet.h"

#include "render/buffer.h"
#include "render/renderer.h"
#include "render/vertex_array.h"

namespace selyan
{
    struct Vertex2d
    {
        glm::vec3 position;
        glm::vec2 textureCoords;
        glm::vec2 normal;
    };

    struct SpriteFrame
    {
        uint32_t row;
        uint32_t column;
        float lifeTime;
    };

    class Sprite
    {
    public:
        explicit Sprite(std::shared_ptr<SpriteSheet> spriteSheet, SpriteFrame firstFrame = { 1, 1, 1 });

        ~Sprite() = default;

        void addSpriteFrame(uint32_t row, uint32_t column, float lifeTime);

        void draw(Shader *program);

    private:
        Vertex2d m_vertices[4];

        VertexBuffer *m_vertexBuffer;
        VertexArray *m_vertexArray;

        glm::mat3 m_textureMatrix;
        std::shared_ptr<SpriteSheet> m_sheet;

        std::vector<SpriteFrame> m_frames;
        uint32_t m_currentFrame;
    };

}
