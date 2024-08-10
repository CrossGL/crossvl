#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
    class OPENGLVertexShader
    {
        friend class Renderer;

    public:
        OPENGLVertexShader()
        {}

    private:
        GLuint vertexShader;
    };
} // namespace CGL::Graphics
