#pragma once

#include "render/texture_2d.h"

#include <glad/glad.h>

#include <cstdint>

namespace selyan
{
    class OGLTexture2D : public Texture2D
    {
    public:
        OGLTexture2D();

        ~OGLTexture2D();

        uint32_t getNative() override;

        uint32_t getWidth() override;

        uint32_t getHeight() override;

        void TextureData(uint32_t width,
                         uint32_t height,
                         uint32_t channels,
                         TextureDataType dataType,
                         void *data) override;

        virtual void TextureData(uint32_t width, uint32_t height, void *data) override;

        void TextureData(Image *image) override;

        void textureSubData(uint32_t widthOffset,
                            uint32_t heightOffset,
                            uint32_t width,
                            uint32_t height,
                            void *data) override;

        void *getTextureData() override;

        void bind() override;

        void unbind() override;

        uint32_t getTexUnit() const override { return m_texUnit; }

        void setTexUnit(uint32_t unit) override;

    private:
        GLuint m_index;

        GLuint m_texUnit;

        GLenum m_internalFormat;
        GLenum m_dataFormat;
        GLenum m_dataType;
    };

}
