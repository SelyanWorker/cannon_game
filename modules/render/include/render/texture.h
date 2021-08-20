#pragma once

#include "utility/image.h"

namespace selyan
{
    enum class TextureDataType
    {
        unsigned_byte,
        floating_point
    };

    class Texture
    {
    public:
        virtual void TextureData(uint32_t width,
                                 uint32_t height,
                                 uint32_t channels,
                                 TextureDataType dataType,
                                 void *data) = 0;

        virtual void TextureData(uint32_t width, uint32_t height, void *data) = 0;

        virtual void TextureData(Image *image) = 0;

        virtual void textureSubData(uint32_t widthOffset,
                                    uint32_t heightOffset,
                                    uint32_t width,
                                    uint32_t height,
                                    void *data) = 0;

        virtual void *getTextureData() = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual uint32_t getTexUnit() const = 0;

        virtual uint32_t getNative() = 0;

        virtual void setTexUnit(uint32_t unit) = 0;
    };

}
