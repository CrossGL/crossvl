#pragma once
#include <Core/Graphics/Types.h>
#include <GL/glew.h>

namespace CGL::Graphics
{
    struct OPENGLVertexBuffer
    {
        u32 Size;
        u32 Offset;
        GLuint VBO;
        GLuint VAO;
    };
} // namespace CGL::Graphics
