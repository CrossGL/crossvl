#pragma once
#include <Core/Application/Application.h>
#include <Core/Common.h>

namespace CGL
{
    CGL_DECLARE_LOG_CATEGORY(HelloTriangle);

    class HelloTriangle : public Core::Application
    {
    protected:
        using Super = Core::Application;

    public:
        HelloTriangle(i32 argc, char** argv);
        ~HelloTriangle();

    private:
        bool OnInit() override final;
        void OnUpdate(const SDL_Event& e) override final;
        void OnRender() override final;
        void OnResize(u32 width, u32 height) override final;
        void OnShutdown() override final;

    private:
        Graphics::VertexBuffer m_vertexBuffer;
        Graphics::Material m_material;
    };
}  // namespace CGL
