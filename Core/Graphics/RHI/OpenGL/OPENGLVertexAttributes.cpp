#include "OPENGLVertexAttributes.h"

namespace CGL::Graphics
{
    template <>
    void OPENGLEnableVertexAttributes<VertexTypes::Position>()
    {
        glEnableVertexAttribArray(0); // Position Attribute
        glVertexAttribPointer(
            0,
            sizeof(Graphics::VertexTypes::Position::Position) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::Position),
            (void*)offsetof(Graphics::VertexTypes::Position, Position)
        );
    }

    template <>
    void OPENGLEnableVertexAttributes<VertexTypes::PositionColor>() {
        glEnableVertexAttribArray(0); // Position Attribute
        glVertexAttribPointer(
            0,
            sizeof(Graphics::VertexTypes::PositionColor::Position) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionColor),
            (void*)offsetof(Graphics::VertexTypes::PositionColor, Position)
        );

        glEnableVertexAttribArray(1); // Color Attribute
        glVertexAttribPointer(
            1,
            sizeof(Graphics::VertexTypes::PositionColor::Color) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionColor),
            (void*)offsetof(Graphics::VertexTypes::PositionColor, Color)
        );
    }
}