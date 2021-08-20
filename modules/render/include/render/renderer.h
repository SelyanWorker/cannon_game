#pragma once

#include "vertex_array.h"

namespace selyan
{
    enum RenderMode
    {
        RN_POINTS = 0x0000,
        RN_LINES = 0x0001,
        RN_LINE_LOOP = 0x0002,
        RN_LINE_STRIP = 0x0003,
        RN_TRIANGLES = 0x0004,
        RN_TRIANGLE_STRIP = 0x0005,
        RN_TRIANGLE_FAN = 0x0006,
        RN_PATCHES = 0x000E
    };

    void Setup();

    void Clear();

    void DrawArray(RenderMode mode, uint32_t vertexArray, uint32_t count);

    void DrawElements(RenderMode mode, uint32_t vertexArray, uint32_t count);

    uint32_t CreateBuffer();

    void BufferData(uint32_t buffer, void *dataPtr, size_t dataSize);

    void BufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data);

    void IndicesBufferData(uint32_t buffer, void *dataPtr, size_t dataSize);

    void IndicesBufferSubData(uint32_t buffer, int64_t offset, int64_t size, void *data);

    uint32_t CreateVertexArray();

    void BufferAttach(uint32_t vertexArray,
                                uint32_t buffer,
                                uint32_t index = 0,
                                uint32_t size = 3,
                                uint32_t stride = 0,
                                void *pointer = nullptr);

    void IndicesBufferAttach(uint32_t vertexArray, uint32_t buffer);

    void EnableRestartIndex(uint32_t index);

    void DisableRestartIndex();

    void EnablePolygonMode();

    void DisablePolygonMode();

    void EnableCullFace();

    void DisableCullFace();

    void SetPointSize(float size);

    void SetLineWidth(float width);

    float GetPointSize();

    float GetLineWidth();

    void DeleteBuffer(uint32_t buffer);

    void SetPathSize(uint32_t size);

    void RendererTest();

    // new
    void DrawVertexArray(RenderMode mode, VertexArray *array);

}
