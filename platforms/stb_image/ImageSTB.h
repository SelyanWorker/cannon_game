#pragma once

#include "utility/image.h"

namespace selyan
{
    class ImageSTB : public Image
    {
    public:
        static ImageSTB *Create(const char *name);

        static ImageSTB *Create(uint8_t const *pixels,
                                uint32_t channels,
                                uint32_t width,
                                uint32_t height);

        ~ImageSTB();

        uint32_t getWidth() const override;

        uint32_t getHeight() const override;

        uint32_t getChannels() const override;

        uint8_t *getData() const override;

        ImageTypes getType() const override;

        bool saveImage(char const *file_name, ImageTypes Type) override;

        void rescale(size_t width, size_t height) override;

    private:
        ImageSTB() : m_width(0), m_height(0), m_channels(0), m_data(nullptr) {}

    private:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_channels;

        uint8_t *m_data;
    };

}
