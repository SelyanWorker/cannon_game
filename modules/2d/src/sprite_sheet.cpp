
#include "2d/SpriteSheet.h"

namespace selyan
{
    SpriteSheet::SpriteSheet(Texture2D *texture, uint32_t rowsNumber, uint32_t columnsNumber)
      : m_texture(texture),
        m_rowsNumber(rowsNumber),
        m_columnsNumber(columnsNumber)
    {
        m_spriteSize = { float(m_texture->getWidth()) / m_columnsNumber,
                         float(m_texture->getHeight()) / m_rowsNumber };
    }

    SpriteSheet::~SpriteSheet() { delete m_texture; }

    void SpriteSheet::Bind() { m_texture->bind(); }

    uint32_t SpriteSheet::GetWidth() const { return m_texture->getWidth(); }

    uint32_t SpriteSheet::GetHeight() const { return m_texture->getHeight(); }

    uint32_t SpriteSheet::GetRowsNumber() const { return m_rowsNumber; }

    uint32_t SpriteSheet::GetColumnsNumber() const { return m_columnsNumber; }

    Vector2f SpriteSheet::GetSpriteSize() const { return m_spriteSize; }

    Matrix3f SpriteSheet::GetTranslationMatrix(uint32_t row, uint32_t column) const
    {
        Matrix3f matrix;
        matrix[0][2] = m_spriteSize.x * column / m_texture->getWidth();
        matrix[1][2] = m_spriteSize.y * row / m_texture->getHeight();
        return matrix;
    }

}
