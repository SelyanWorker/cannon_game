#include "sprite/sprite.h"
#include "core/time.h"

namespace selyan
{
    Sprite::Sprite(std::shared_ptr<SpriteGeometry> geometry,
                   std::shared_ptr<SpriteSheet> spriteSheet,
                   SpriteFrame firstFrame)
    :   m_geometry(std::move(geometry)),
        m_sheet(std::move(spriteSheet)),
        m_currentFrame(0),
        m_lastFrameLiftTime(0),
        m_animationDone(false)
    {
        m_frames.push_back(firstFrame);
        m_textureMatrix =
            m_sheet->getTextureTranslationMatrix(m_frames.back().row, m_frames.back().column);

    }

    void Sprite::addSpriteFrame(uint32_t row, uint32_t column, float lifeTime)
    {
        m_frames.push_back({ row, column, lifeTime });
    }

    void Sprite::update(float elapsedTime)
    {
        if(m_frames.size() == 1)
            return;

        m_lastFrameLiftTime += elapsedTime;
        m_animationDone = false;

        if (m_frames[m_currentFrame].lifeTime <= m_lastFrameLiftTime)
        {
            m_lastFrameLiftTime = 0;
            if (++m_currentFrame >= m_frames.size())
            {
                m_currentFrame = 0;
                m_animationDone = true;
            }
            const auto& nextFrame = m_frames[m_currentFrame];
            m_textureMatrix =
                m_sheet->getTextureTranslationMatrix(nextFrame.row, nextFrame.column);
        }

    }

    void Sprite::draw(std::shared_ptr<selyan::Shader> shader)
    {
        shader->setUniform("textureMatrix", m_textureMatrix);

        m_sheet->bind();

        m_geometry->draw();
    }

    bool Sprite::animationDone() const
    {
        return m_animationDone;
    }

    void Sprite::reset()
    {
        m_animationDone = false;
        m_currentFrame = 0;
        m_lastFrameLiftTime = 0;
    }

}
