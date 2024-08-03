#pragma once
#include <Core/Graphics/Shader/Shader.h>
#include <Core/Graphics/Shader/ShaderSource.h>
#include <Core/Logging/Log.h>

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(Material);

    enum class ShaderState
    {
        Invalid,
        CompilePending,
        Compiled,
    };

    template <typename T>
    struct ShaderProgram
    {
        ShaderState State;
        ShaderSource Source;
        T Shader;
    };

    class Material
    {
        friend class Renderer;
    public:
#if defined(CGL_RHI_OPENGL)
        using ID = GLuint;
#else
        using ID = u32;
#endif
        Material();
        ~Material();

        // Add a shader source to this material (before compilation)
        void AddSource(const ShaderSource& source);

        inline ID GetMaterialID() const noexcept { return m_id; }
        inline const ShaderProgram<VertexShader>* GetVertexShader() const noexcept { return m_vs.get(); }
        inline const ShaderProgram<PixelShader>* GetPixelShader() const noexcept { return m_ps.get(); }

    private:
        ID                                           m_id;
        std::unique_ptr<ShaderProgram<VertexShader>> m_vs;
        std::unique_ptr<ShaderProgram<PixelShader>>  m_ps;
        // TODO: Add other shader types
    };
}