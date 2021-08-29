#include "render/buffer.h"
#include "render/renderer.h"

#include <cassert>
#include <glad/glad.h>

namespace selyan
{
    void setup()
    {
        glClearColor(0.8f, 0.8f, 0.8f, 1.f);
        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    uint32_t createBuffer()
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        return vbo;
    }

    void bufferData(uint32_t buffer, void *dataPtr, size_t dataSize)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, dataPtr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void bufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void indicesBufferData(uint32_t buffer, void *dataPtr, size_t dataSize)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, dataPtr, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void indicesBufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t createVertexArray()
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        return vao;
    }

    void bufferAttach(uint32_t vertexArray,
                      uint32_t buffer,
                      uint32_t index,
                      uint32_t size,
                      uint32_t stride,
                      void *pointer)
    {
        //        RN_ASSERT(vertexArray != 0 && buffer != 0,
        //                  "VertexArray == 0 or Buffer == 0 in \"bufferAttach\"");
        assert(vertexArray != 0 && buffer != 0 &&
               "VertexArray == 0 or Buffer == 0 in \"bufferAttach\"");
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(index);
        glBindVertexArray(0);
    }

    void indicesBufferAttach(uint32_t vertexArray, uint32_t buffer)
    {
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBindVertexArray(0);
    }

    void drawArray(RenderMode mode, uint32_t vertexArray, uint32_t count)
    {
        glBindVertexArray(vertexArray);
        glDrawArrays(mode, 0, count);
        glBindVertexArray(0);
    }

    void drawElements(RenderMode mode, uint32_t vertexArray, uint32_t count)
    {
        glBindVertexArray(vertexArray);
        glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void enableRestartIndex(uint32_t index)
    {
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(index);
    }

    void disableRestartIndex() { glDisable(GL_PRIMITIVE_RESTART); }

    void enablePolygonMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }

    void disablePolygonMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    void enableCullFace() { glEnable(GL_CULL_FACE); }

    void disableCullFace() { glDisable(GL_CULL_FACE); }

    void setPointSize(float size) { glPointSize(size); }

    void setLineWidth(float width) { glLineWidth(width); }

    float getPointSize()
    {
        GLfloat size;
        glGetFloatv(GL_POINT_SIZE, &size);
        return size;
    }

    float getLineWidth()
    {
        GLfloat width;
        glGetFloatv(GL_LINE_WIDTH, &width);
        return width;
    }

    void deleteBuffer(uint32_t buffer) { glDeleteBuffers(1, &buffer); }

    void rendererTest()
    {
        GLboolean res;
        glGetBooleanv(GL_DEPTH_TEST, &res);
        // RN_CORE_INFO("rendererTest GL_DEPTH_TEST enable: {0}", res);
    }

    // new
    void drawVertexArray(RenderMode mode, VertexArray *vertexArray)
    {
        vertexArray->bind();

        if (vertexArray->getIndexBuffer() != nullptr)
            glDrawElementsInstanced(mode,
                                    vertexArray->getIndexBuffer()->getCount(),
                                    GL_UNSIGNED_INT,
                                    nullptr,
                                    vertexArray->getInstanceCount());
        else
        {
            GLuint count = vertexArray->getVertexBuffers()[0]->getBufferLayout().getVertexCount();
            glDrawArraysInstanced(mode, 0, count, vertexArray->getInstanceCount());
        }

        vertexArray->unbind();
    }
}
