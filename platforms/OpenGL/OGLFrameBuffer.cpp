#include "OGLFrameBuffer.h"
#include <iostream>

namespace selyan
{
    FrameBuffer *FrameBuffer::create(std::initializer_list<Texture2D *> colorTextures)
    {
        GLint maxAttach = 0;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
        if (GLint(colorTextures.size()) > maxAttach)
        {
            //RN_CORE_ERROR("FrameBuffer::Create - too much color textures");
            std::cout << "FrameBuffer::Create - too much color textures" << std::endl;
            return nullptr;
        }

        bool equalTexturesSize = true;
        // need check equal texture size
        if (!equalTexturesSize)
        {
            //RN_CORE_ERROR("FrameBuffer::Create - texture's size not equal");
            std::cout << "FrameBuffer::Create - texture's size not equal" << std::endl;
            return nullptr;
        }

        return new OGLFrameBuffer(colorTextures);
    }

    OGLFrameBuffer::OGLFrameBuffer(std::initializer_list<Texture2D *> colorTextures)
      : m_index(0),
        m_viewportData(new GLint[4])
    {
        for (auto texturePtr : colorTextures)
            m_colorTextures.push_back(static_cast<OGLTexture2D *>(texturePtr));

        m_width = m_colorTextures[0]->GetWidth();
        m_height = m_colorTextures[0]->GetHeight();

        glGenFramebuffers(1, &m_index);
        glBindFramebuffer(GL_FRAMEBUFFER, m_index);

        // depth texture
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH_COMPONENT,
                     m_width,
                     m_height,
                     0,
                     GL_DEPTH_COMPONENT,
                     GL_UNSIGNED_BYTE,
                     nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D,
                               m_depthTexture,
                               0);

        // color textures
        std::vector<GLenum> drawColorTextures;
        for (size_t i = 0; i < m_colorTextures.size(); i++)
        {
            drawColorTextures.push_back(GL_COLOR_ATTACHMENT0 + i);
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   drawColorTextures.back(),
                                   GL_TEXTURE_2D,
                                   m_colorTextures[i]->getNative(),
                                   0);
        }

        glDrawBuffers(drawColorTextures.size(), drawColorTextures.data());

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            //RN_CORE_ERROR("Framebuffer status - not complete");
            std::cout << "Framebuffer status - not complete" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OGLFrameBuffer::~OGLFrameBuffer()
    {
      unbind();
        glDeleteFramebuffers(1, &m_index);
    }

    void OGLFrameBuffer::bind()
    {
        glGetIntegerv(GL_VIEWPORT, m_viewportData);
        glBindFramebuffer(GL_FRAMEBUFFER, m_index);
        glViewport(0, 0, m_width, m_height);
    }

    void OGLFrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(m_viewportData[0], m_viewportData[1], m_viewportData[2], m_viewportData[3]);
    }

    void OGLFrameBuffer::setSize(float width, float height)
    {
        m_width = width;
        m_height = height;

        // reallocate depth texture
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH_COMPONENT,
                     m_width,
                     m_height,
                     0,
                     GL_DEPTH_COMPONENT,
                     GL_UNSIGNED_BYTE,
                     nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        // reallocate color texture
        for (auto &texturePtr : m_colorTextures)
        {
          texturePtr->bind();
            texturePtr->TextureData(m_width, m_height, nullptr);
            texturePtr->unbind();
        }
    }

    std::pair<float, float> OGLFrameBuffer::getSize() { return { m_width, m_height }; }

    Texture2D *OGLFrameBuffer::getAttachmentColorTexture(uint32_t number)
    {
        return m_colorTextures.size() > number ? m_colorTextures[number] : nullptr;
    }

    void *OGLFrameBuffer::getColorData(uint32_t number)
    {
        number++;
        /*Vector4f* data = new Vector4f[m_width * m_height];
        glBindTexture(GL_TEXTURE_2D, m_textures[1 + number]);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, );
        glBindTexture(GL_TEXTURE_2D, 0);*/
        return nullptr;
    }

}
