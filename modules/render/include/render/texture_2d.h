#pragma once

#include "texture.h"
#include "utility/image.h"

#include <cstdint>

namespace selyan
{
    class Texture2D : public Texture
    {
    public:
        virtual void textureData(uint32_t width,
                                 uint32_t height,
                                 uint32_t channels,
                                 TextureDataType dataType,
                                 void *data) = 0;

        virtual void textureData(uint32_t width, uint32_t height, void *data) = 0;

        virtual void textureData(Image *image) = 0;

        virtual void textureSubData(uint32_t widthOffset,
                                    uint32_t heightOffset,
                                    uint32_t width,
                                    uint32_t height,
                                    void *data) = 0;

        virtual void *getTextureData() = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual uint32_t getTexUnit() const = 0;

        virtual void setTexUnit(uint32_t unit) = 0;

        virtual uint32_t getWidth() = 0;

        virtual uint32_t getHeight() = 0;

        static Texture2D *create();

    protected:
        uint32_t m_width;
        uint32_t m_height;
    };

}
