#pragma once
#include <Core/Application/Application.h>
#include <Core/Common.h>
#include <Core/Graphics/Camera.h>

namespace CGL
{
    CGL_DECLARE_LOG_CATEGORY(SpinningCube);

    struct FrameData
    {
        SM::Matrix World;
        SM::Matrix View;
        SM::Matrix Projection;
    };

    class SpinningCube : public Core::Application
    {
    protected:
        using Super = Core::Application;

    public:
        SpinningCube(i32 argc, char** argv);
        ~SpinningCube();

    private:
        bool OnInit() override final;
        void OnUpdate(const SDL_Event& e) override final;
        void OnRender() override final;
        void OnResize(u32 width, u32 height) override final;
        void OnShutdown() override final;

    private:
        Graphics::ConstantBuffer<FrameData> m_constantBuffer;
        Graphics::VertexBuffer m_vertexBuffer;
        Graphics::IndexBuffer m_indexBuffer;
        Graphics::Material m_material;
        Graphics::Camera m_camera;
    };
}  // namespace CGL
