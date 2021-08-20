
#include "sprite/sprite_sheet.h"

namespace selyan
{
    SpriteSheet::SpriteSheet(Texture2D *texture, uint32_t rowsNumber, uint32_t columnsNumber)
      : m_texture(texture),
        m_rowsNumber(rowsNumber),
        m_columnsNumber(columnsNumber)
    {
        m_spriteSize = { float(m_texture->GetWidth()) / m_columnsNumber,
                         float(m_texture->GetHeight()) / m_rowsNumber };
    }

    SpriteSheet::~SpriteSheet() { delete m_texture; }

    void SpriteSheet::bind() { m_texture->bind(); }

    uint32_t SpriteSheet::getWidth() const { return m_texture->GetWidth(); }

    uint32_t SpriteSheet::getHeight() const { return m_texture->GetHeight(); }

    uint32_t SpriteSheet::getRowsNumber() const { return m_rowsNumber; }

    uint32_t SpriteSheet::getColumnsNumber() const { return m_columnsNumber; }

    glm::vec2 SpriteSheet::getSpriteSize() const { return m_spriteSize; }

    glm::mat3 SpriteSheet::getTranslationMatrix(uint32_t row, uint32_t column) const
    {
        glm::mat3 matrix;
        matrix[0][2] = m_spriteSize.x * column / m_texture->GetWidth();
        matrix[1][2] = m_spriteSize.y * row / m_texture->GetHeight();
        return matrix;
    }

}
