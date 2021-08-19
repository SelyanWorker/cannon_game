
#include "2d/SpriteSheet.h"

namespace Rainy
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

    void SpriteSheet::Bind() { m_texture->Bind(); }

    uint32_t SpriteSheet::GetWidth() const { return m_texture->GetWidth(); }

    uint32_t SpriteSheet::GetHeight() const { return m_texture->GetHeight(); }

    uint32_t SpriteSheet::GetRowsNumber() const { return m_rowsNumber; }

    uint32_t SpriteSheet::GetColumnsNumber() const { return m_columnsNumber; }

    Vector2f SpriteSheet::GetSpriteSize() const { return m_spriteSize; }

    Matrix3f SpriteSheet::GetTranslationMatrix(uint32_t row, uint32_t column) const
    {
        Matrix3f matrix;
        matrix[0][2] = m_spriteSize.x * column / m_texture->GetWidth();
        matrix[1][2] = m_spriteSize.y * row / m_texture->GetHeight();
        return matrix;
    }

}
