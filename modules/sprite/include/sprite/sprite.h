#pragma once

#include "sprite_geometry.h"
#include "sprite_sheet.h"

#include "render/shader.h"
#include "render/texture_2d.h"
#include "render/buffer.h"
#include "render/renderer.h"
#include "render/vertex_array.h"

namespace selyan
{
    struct SpriteFrame
    {
        uint32_t row;
        uint32_t column;
        float lifeTime;
    };

    class Sprite
    {
    public:
        explicit Sprite(std::shared_ptr<SpriteGeometry> geometry,
                        std::shared_ptr<SpriteSheet> spriteSheet,
                        SpriteFrame firstFrame = { 0, 0, 0 });

        ~Sprite() = default;

        void addSpriteFrame(uint32_t row, uint32_t column, float lifeTime);

        void update(float elapsedTime);

        void draw(std::shared_ptr<selyan::Shader> shader);

        bool animationDone() const;

        void reset();

    private:
        std::shared_ptr<SpriteGeometry> m_geometry;

        glm::mat3 m_textureMatrix;
        std::shared_ptr<SpriteSheet> m_sheet;

        std::vector<SpriteFrame> m_frames;
        uint32_t m_currentFrame;
        float m_lastFrameLiftTime;

        bool m_animationDone;
    };

}
