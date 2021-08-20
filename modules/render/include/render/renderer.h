#pragma once

#include "vertex_array.h"

namespace selyan
{
    enum RenderMode
    {
        points = 0x0000,
        lines = 0x0001,
        line_loop = 0x0002,
        line_strip = 0x0003,
        triangles = 0x0004,
        triangle_strip = 0x0005,
        triangle_fan = 0x0006,
        patches = 0x000E
    };

    void setup();

    void clear();

    void drawArray(RenderMode mode, uint32_t vertexArray, uint32_t count);

    void drawElements(RenderMode mode, uint32_t vertexArray, uint32_t count);

    uint32_t createBuffer();

    void bufferData(uint32_t buffer, void *dataPtr, size_t dataSize);

    void bufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data);

    void indicesBufferData(uint32_t buffer, void *dataPtr, size_t dataSize);

    void indicesBufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data);

    uint32_t createVertexArray();

    void bufferAttach(uint32_t vertexArray,
                                uint32_t buffer,
                                uint32_t index = 0,
                                uint32_t size = 3,
                                uint32_t stride = 0,
                                void *pointer = nullptr);

    void indicesBufferAttach(uint32_t vertexArray, uint32_t buffer);

    void enableRestartIndex(uint32_t index);

    void disableRestartIndex();

    void enablePolygonMode();

    void disablePolygonMode();

    void enableCullFace();

    void disableCullFace();

    void setPointSize(float size);

    void setLineWidth(float width);

    float getPointSize();

    float getLineWidth();

    void deleteBuffer(uint32_t buffer);

    void setPathSize(uint32_t size);

    void rendererTest();

    // new
    void drawVertexArray(RenderMode mode, VertexArray *array);

}
