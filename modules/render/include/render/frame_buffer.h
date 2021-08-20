#pragma once

#include "texture_2d.h"

#include <initializer_list>
#include <utility>

namespace selyan
{
    class FrameBuffer
    {
    public:
        static FrameBuffer *create(std::initializer_list<Texture2D *> colorTextures);

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setSize(float width, float height) = 0;

        virtual std::pair<float, float> getSize() = 0;

        virtual Texture2D *getAttachmentColorTexture(uint32_t number) = 0;

        virtual void *getColorData(uint32_t number) = 0;
    };

}
