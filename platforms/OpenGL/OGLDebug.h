#pragma once

#include "glad/glad.h"

namespace selyan
{
    GLenum CheckError_(int line);
#define CheckError() CheckError_(__LINE__)

    void APIENTRY OGLDebugOutput(GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar *message,
                                 const void *userParam);

}