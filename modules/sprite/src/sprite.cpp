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
        m_lastFrameLiftTime(0)
    {
        m_frames.push_back(firstFrame);
        m_textureMatrix =
            m_sheet->getTextureTranslationMatrix(m_frames.back().row, m_frames.back().column);

//        glm::vec2 textureSize = m_sheet->getSpriteSize();
//        textureSize.x /= m_sheet->getWidth();
//        textureSize.y /= m_sheet->getHeight();
//
//        const float aspectRation = m_sheet->getSpriteSize().x / m_sheet->getSpriteSize().y;
//        constexpr float halfOfSpriteSide = 0.5;
//        constexpr float z = -1;
//
//        m_vertices[0] = { { -halfOfSpriteSide * aspectRation, -halfOfSpriteSide, z }, { 0, 0 }, { 0, 0 } };               // bottom left
//        m_vertices[1] = { { -halfOfSpriteSide * aspectRation, halfOfSpriteSide, z }, { 0, textureSize.y }, { 0, 0 } };   // top left
//        m_vertices[2] = { { halfOfSpriteSide * aspectRation, -halfOfSpriteSide, z }, { textureSize.x, 0 }, { 0, 0 } };   // bottom right
//        m_vertices[3] = { { halfOfSpriteSide * aspectRation, halfOfSpriteSide, z }, textureSize, { 0, 0 } };            // top right
//
//        m_vertexBuffer = VertexBuffer::create(sizeof(Vertex2d) * 4, m_vertices);
//        m_vertexArray = VertexArray::create();
//
//        auto elements = {
//            BufferElement(float3, false),
//            BufferElement(float2, false),
//            BufferElement(float2, false),
//        };
//        m_vertexBuffer->setBufferLayout({ elements, 4 });
//
//        m_vertexArray->setVertexBuffers({ m_vertexBuffer });
    }

    //Sprite::~Sprite() { delete m_sheet; }

    void Sprite::addSpriteFrame(uint32_t row, uint32_t column, float lifeTime)
    {
        m_frames.push_back({ row, column, lifeTime });
    }

    void Sprite::update(float elapsedTime)
    {
        if(m_frames.size() == 1)
            return;

        m_lastFrameLiftTime += elapsedTime;

        std::cout << m_frames[m_currentFrame].lifeTime << " " << m_lastFrameLiftTime << std::endl;

        if (m_frames[m_currentFrame].lifeTime <= m_lastFrameLiftTime)
        {
            m_lastFrameLiftTime = 0;
            if (++m_currentFrame >= m_frames.size())
                m_currentFrame = 0;
            const auto& nextFrame = m_frames[m_currentFrame];
            m_textureMatrix =
                m_sheet->getTextureTranslationMatrix(nextFrame.row, nextFrame.column);
        }
    }

    void Sprite::draw(Shader *program)
    {
        // frame - sprite animation frame
//        float currentTime = TimeStep::getTime().getSeconds();
//
//        static float timeFromStart = currentTime;
//        static float frameTime = currentTime - timeFromStart;
//
//        SpriteFrame currentFrame = m_frames[m_currentFrame];
//        if (currentFrame.lifeTime < frameTime)
//        {
//            timeFromStart = currentTime;
//            m_currentFrame += 1;
//            if (m_currentFrame >= m_frames.size())
//                m_currentFrame = 0;
//            currentFrame = m_frames[m_currentFrame];
//
//            m_textureMatrix =
//                m_sheet->getTextureTranslationMatrix(currentFrame.row, currentFrame.column);
//        }
//
//        frameTime = currentTime - timeFromStart;

        program->setUniform("textureMatrix", m_textureMatrix);

        m_sheet->bind();

        m_geometry->draw();
    }

}
