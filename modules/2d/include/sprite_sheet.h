#pragma once

#include "core/core.h"
#include "math/math.h"
#include "render/texture_2d.h"

namespace selyan
{
    class SpriteSheet
    {
    public:
        explicit SpriteSheet(Texture2D *texture, uint32_t rowsNumber, uint32_t columnsNumber);

        ~SpriteSheet();

        void Bind();

        uint32_t GetWidth() const;

        uint32_t GetHeight() const;

        uint32_t GetRowsNumber() const;

        uint32_t GetColumnsNumber() const;

        Vector2f GetSpriteSize() const;

        Matrix3f GetTranslationMatrix(uint32_t row, uint32_t column) const;

    private:
        Texture2D *m_texture;

        Vector2f m_spriteSize;
        uint32_t m_rowsNumber;
        uint32_t m_columnsNumber;
    };

}
