
#include "ImageSTB.h"
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <cstring>
#include <cstdint>

namespace selyan
{
    Image *Image::create(char const *name) { return ImageSTB::create(name); }

    Image *Image::create(uint8_t const *pixels, uint32_t channels, uint32_t width, uint32_t height)
    {
        return ImageSTB::create(pixels, channels, width, height);
    }

    ImageSTB *ImageSTB::create(const char *name)
    {
        int width;
        int height;
        int channels;
        stbi_set_flip_vertically_on_load(1);

        ImageSTB *image = new ImageSTB;
        image->m_data = stbi_load(name, &width, &height, &channels, 0);
        if (image->m_data == nullptr)
        {
            delete image;
            return nullptr;
        }

        image->m_width = width;
        image->m_height = height;
        image->m_channels = channels;
        return image;
    }

    ImageSTB *ImageSTB::create(uint8_t const *pixels,
                               uint32_t channels,
                               uint32_t width,
                               uint32_t height)
    {
        ImageSTB *image = new ImageSTB;
        image->m_data = new uint8_t[width * height * channels];
        memcpy(image->m_data, pixels, width * height * channels);
        image->m_width = width;
        image->m_height = height;
        image->m_channels = channels;
        return image;
    }

    ImageSTB::~ImageSTB() { stbi_image_free(m_data); }

    uint32_t ImageSTB::getWidth() const { return m_width; }

    uint32_t ImageSTB::getHeight() const { return m_height; }

    uint32_t ImageSTB::getChannels() const { return m_channels; }

    uint8_t *ImageSTB::getData() const { return m_data; }

    ImageTypes ImageSTB::getType() const { return ImageTypes::png; }

    bool ImageSTB::saveImage(char const *fileName, ImageTypes Type)
    {
        int result = 0;
        if (Type == ImageTypes::png)
            result = stbi_write_png(fileName,
                                    m_width,
                                    m_height,
                                    m_channels,
                                    m_data,
                                    m_width * m_channels);
        else
            result = stbi_write_jpg(fileName, m_width, m_height, m_channels, m_data, 100);
        if (result == 0)
            return false;
        return true;
    }

    void ImageSTB::rescale(size_t width, size_t height) {}

}
