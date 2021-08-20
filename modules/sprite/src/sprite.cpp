#include "core/time.h"
#include "sprite/sprite.h"

namespace selyan
{
    Sprite::Sprite(SpriteSheet *spriteSheet, SpriteFrame firstFrame)
      : m_sheet(spriteSheet),
        m_currentFrame(0)
    {
        m_frames.push_back(firstFrame);
        m_textureMatrix =
            m_sheet->getTranslationMatrix(m_frames.back().row, m_frames.back().column);

        glm::vec2 textureSize = m_sheet->getSpriteSize();
        textureSize.x /= m_sheet->getWidth();
        textureSize.y /= m_sheet->getHeight();

//        m_vertices[0] = { { 0, 0, 0 }, { 0, 0 }, { 0, 0 } };               // bottom left
//        m_vertices[1] = { { 0, 1, 0 }, { 0, textureSize.y }, { 0, 0 } };   // top left
//        m_vertices[2] = { { 1, 0, 0 }, { textureSize.x, 0 }, { 0, 0 } };   // bottom right
//        m_vertices[3] = { { 1, 1, 0 }, textureSize, { 0, 0 } };            // top right

        m_vertices[0] = { { 0, 0, 0 }, { 0, 0 }, { 0, 0 } };               // bottom left
        m_vertices[1] = { { 0, 1, 0 }, { 0, textureSize.y }, { 0, 0 } };   // top left
        m_vertices[2] = { { 1, 0, 0 }, { textureSize.x, 0 }, { 0, 0 } };   // bottom right
        m_vertices[3] = { { 1, 1, 0 }, textureSize, { 0, 0 } };            // top right

        m_vertexBuffer = VertexBuffer::create(sizeof(Vertex2d) * 4, m_vertices);
        m_vertexArray = VertexArray::create();

        auto elements = {
            BufferElement(FLOAT3, false),
            BufferElement(FLOAT2, false),
            BufferElement(FLOAT2, false),
        };
        m_vertexBuffer->setBufferLayout({ elements, 4 });

        m_vertexArray->setVertexBuffers({ m_vertexBuffer });
        /*BufferAttach(m_vertexArray, m_vertexBuffer, 0, 3, sizeof(Vertex2D), (void*)0);
        BufferAttach(m_vertexArray, m_vertexBuffer, 1, 2, sizeof(Vertex2D),
        (void*)(offsetof(Vertex2D, textCoord))); BufferAttach(m_vertexArray, m_vertexBuffer, 2, 2,
        sizeof(Vertex2D), (void*)(offsetof(Vertex2D, normal)));*/
    }

    Sprite::~Sprite() { delete m_sheet; }

    void Sprite::addSpriteFrame(uint32_t row, uint32_t column, float lifeTime)
    {
        m_frames.push_back({ row, column, lifeTime });
    }

    void Sprite::draw(Shader *program)
    {
        // frame - sprite animation frame
        float currentTime = TimeStep::getTime().getSeconds();

        static float timeFromStart = currentTime;
        static float frameTime = currentTime - timeFromStart;

        SpriteFrame currentFrame = m_frames[m_currentFrame];
        if (currentFrame.lifeTime < frameTime)
        {
            timeFromStart = currentTime;
            m_currentFrame += 1;
            if (m_currentFrame >= m_frames.size())
                m_currentFrame = 0;
            currentFrame = m_frames[m_currentFrame];

            m_textureMatrix = m_sheet->getTranslationMatrix(currentFrame.row, currentFrame.column);
        }

        frameTime = currentTime - timeFromStart;

        program->setUniform("textureMatrix", m_textureMatrix);

        m_sheet->bind();

        // DrawArray(RenderMode::RN_TRIANGLE_STRIP, m_vertexArray, 4);
        drawVertexArray(RenderMode::RN_TRIANGLE_STRIP, m_vertexArray);
    }

}
