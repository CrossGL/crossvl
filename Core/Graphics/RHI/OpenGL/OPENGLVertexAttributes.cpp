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

    template <>
    void OPENGLEnableVertexAttributes<VertexTypes::PositionTexture>() {
        glEnableVertexAttribArray(0); // Position Attribute
        glVertexAttribPointer(
            0,
            sizeof(Graphics::VertexTypes::PositionTexture::Position) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionTexture, Position)
        );

        glEnableVertexAttribArray(1); // Texture Attribute
        glVertexAttribPointer(
            1,
            sizeof(Graphics::VertexTypes::PositionTexture::Texture) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionTexture, Texture)
        );
    }

    template <>
    void OPENGLEnableVertexAttributes<VertexTypes::PositionColorTexture>() {
        glEnableVertexAttribArray(0); // Position Attribute
        glVertexAttribPointer(
            0,
            sizeof(Graphics::VertexTypes::PositionColorTexture::Position) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionColorTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionColorTexture, Position)
        );

        glEnableVertexAttribArray(1); // Color Attribute
        glVertexAttribPointer(
            1,
            sizeof(Graphics::VertexTypes::PositionColorTexture::Color) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionColorTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionColorTexture, Color)
        );

        glEnableVertexAttribArray(2); // Texture Attribute
        glVertexAttribPointer(
            2,
            sizeof(Graphics::VertexTypes::PositionColorTexture::Texture) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionColorTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionColorTexture, Texture)
        );
    }

    template <>
    void OPENGLEnableVertexAttributes<VertexTypes::PositionNormalTexture>() {
        glEnableVertexAttribArray(0); // Position Attribute
        glVertexAttribPointer(
            0,
            sizeof(Graphics::VertexTypes::PositionNormalTexture::Position) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionNormalTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionNormalTexture, Position)
        );

        glEnableVertexAttribArray(1); // Normal Attribute
        glVertexAttribPointer(
            1,
            sizeof(Graphics::VertexTypes::PositionNormalTexture::Normal) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionNormalTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionNormalTexture, Normal)
        );

        glEnableVertexAttribArray(2); // Texture Attribute
        glVertexAttribPointer(
            2,
            sizeof(Graphics::VertexTypes::PositionNormalTexture::Texture) / sizeof(float),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Graphics::VertexTypes::PositionNormalTexture),
            (void*)offsetof(Graphics::VertexTypes::PositionNormalTexture, Texture)
        );
    }
}