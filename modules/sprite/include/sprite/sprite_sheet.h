#pragma once

#include "render/texture_2d.h"

#include <glm/glm.hpp>
#include <memory>

namespace selyan
{
    class SpriteSheet
    {
    public:
        explicit SpriteSheet(std::shared_ptr<Texture2D> texture,
                             uint32_t rowsNumber,
                             uint32_t columnsNumber);

        ~SpriteSheet() = default;

        void bind();

        uint32_t getWidth() const;

        uint32_t getHeight() const;

        uint32_t getRowsNumber() const;

        uint32_t getColumnsNumber() const;

        glm::vec2 getAbsoluteSpriteSize() const;

        glm::vec2 getRelativeSpriteSize() const;

        glm::mat3 getTextureTranslationMatrix(uint32_t row, uint32_t column) const;

        glm::vec2 applySpriteSizeAspectRation(const glm::vec2& value) const;

    private:
        std::shared_ptr<Texture2D> m_texture;

        uint32_t m_rowsNumber;
        uint32_t m_columnsNumber;
        glm::vec2 m_spriteSize;
    };

}
