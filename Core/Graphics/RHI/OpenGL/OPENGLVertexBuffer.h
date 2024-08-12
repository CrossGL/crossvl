#pragma once
#include <Core/Graphics/Types.h>
#include <GL/glew.h>

namespace CGL::Graphics
{
    class OPENGLVertexBuffer
    {
        friend class Renderer;

    public:
        OPENGLVertexBuffer()
        {}
        ~OPENGLVertexBuffer() = default;

    private:
        u32 m_stride;
        u32 m_offset;
        GLuint VBO;
        GLuint VAO;

    };
} // namespace CGL::Graphics
