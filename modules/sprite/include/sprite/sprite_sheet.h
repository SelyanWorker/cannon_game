#pragma once

#include "render/texture_2d.h"

#include <glm/glm.hpp>

namespace selyan
{
    class SpriteSheet
    {
    public:
        explicit SpriteSheet(Texture2D *texture, uint32_t rowsNumber, uint32_t columnsNumber);

        ~SpriteSheet();

        void bind();

        uint32_t getWidth() const;

        uint32_t getHeight() const;

        uint32_t getRowsNumber() const;

        uint32_t getColumnsNumber() const;

        glm::vec2 getSpriteSize() const;

        glm::mat3 getTranslationMatrix(uint32_t row, uint32_t column) const;

    private:
        Texture2D *m_texture;

        glm::vec2 m_spriteSize;
        uint32_t m_rowsNumber;
        uint32_t m_columnsNumber;
    };

}
