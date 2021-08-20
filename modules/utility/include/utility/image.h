#pragma once

#include <cstdint>

namespace selyan
{
    enum class ImageTypes
    {
        RN_PNG,
        RN_JPEG
    };

    class Image
    {
    public:
        static Image *create(char const *file_name);

        static Image *create(uint8_t const *pixels,
                             uint32_t channels,
                             uint32_t width,
                             uint32_t height);

        virtual uint32_t getWidth() const = 0;

        virtual uint32_t getHeight() const = 0;

        virtual uint32_t getChannels() const = 0;

        virtual uint8_t *getData() const = 0;

        virtual ImageTypes getType() const = 0;

        virtual bool saveImage(char const *file_name, ImageTypes Type) = 0;

        virtual void rescale(size_t width, size_t height) = 0;
    };

}
