
#include "sprite/sprite_sheet.h"

#include <glm/gtc/matrix_transform.hpp>
#include <utility>

namespace selyan
{
    SpriteSheet::SpriteSheet(std::shared_ptr<Texture2D> texture,
                             uint32_t rowsNumber,
                             uint32_t columnsNumber)
      : m_texture(std::move(texture)),
        m_rowsNumber(rowsNumber),
        m_columnsNumber(columnsNumber),
        m_spriteSize(float(m_texture->getWidth()) / m_columnsNumber,
                         float(m_texture->getHeight()) / m_rowsNumber)
    {
    }

    void SpriteSheet::bind() { m_texture->bind(); }

    uint32_t SpriteSheet::getWidth() const { return m_texture->getWidth(); }

    uint32_t SpriteSheet::getHeight() const { return m_texture->getHeight(); }

    uint32_t SpriteSheet::getRowsNumber() const { return m_rowsNumber; }

    uint32_t SpriteSheet::getColumnsNumber() const { return m_columnsNumber; }

    glm::vec2 SpriteSheet::getAbsoluteSpriteSize() const { return m_spriteSize; }

    glm::vec2 SpriteSheet::getRelativeSpriteSize() const
    {
        //        glm::vec2 textureSize = m_spriteSize;
        //        textureSize.x /= m_sheet->getWidth();
        //        textureSize.y /= m_sheet->getHeight();
        return { m_spriteSize.x / m_texture->getWidth(), m_spriteSize.y / m_texture->getHeight() };
    }

    glm::mat3 SpriteSheet::getTextureTranslationMatrix(uint32_t row, uint32_t column) const
    {
        glm::mat3 matrix = glm::identity<glm::mat3>();
        matrix[0][2] = m_spriteSize.x * column / m_texture->getWidth();
        matrix[1][2] = m_spriteSize.y * row / m_texture->getHeight();
        return matrix;
    }

    glm::vec2 SpriteSheet::applySpriteSizeAspectRation(const glm::vec2 &value) const
    {
        const float aspectRation =  m_spriteSize.x / m_spriteSize.y;
        return { value.x *  aspectRation, value.y };
    }
}
