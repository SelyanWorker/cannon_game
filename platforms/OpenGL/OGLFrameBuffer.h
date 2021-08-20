#pragma once

#include "render/frame_buffer.h"

#include "OGLTexture2D.h"

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <vector>

namespace selyan
{
    class OGLFrameBuffer : public FrameBuffer
    {
    public:
        OGLFrameBuffer(std::initializer_list<Texture2D *> colorTextures);

        ~OGLFrameBuffer();

        void bind() override;

        void unbind() override;

        void setSize(float width, float height) override;

        std::pair<float, float> getSize() override;

        Texture2D *getAttachmentColorTexture(uint32_t number) override;

        void *getColorData(uint32_t number) override;

    private:
        GLsizei m_width;
        GLsizei m_height;

        GLuint m_index;
        // std::vector<GLuint> m_textures; // 0 - depth texture, 1 to MAX_ATTACH - color textures
        std::vector<OGLTexture2D *> m_colorTextures;
        GLuint m_depthTexture;
        GLint *m_viewportData;
    };

}
