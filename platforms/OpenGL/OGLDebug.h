#pragma once

#include "glad/glad.h"

namespace selyan
{
    GLenum checkError_(int line);
#define checkError() checkError_(__LINE__)

    void APIENTRY OGLDebugOutput(GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar *message,
                                 const void *userParam);

}