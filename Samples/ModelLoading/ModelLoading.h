#pragma once
#include <Core/Common.h>
#include <Core/Application/Application.h>
#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Camera.h>

namespace CGL
{
	CGL_DECLARE_LOG_CATEGORY(ModelLoading);

	struct FrameData
	{
		SM::Matrix World;
		SM::Matrix View;
		SM::Matrix Projection;
	};

	class ModelLoading : public Core::Application
	{
	protected:
		using Super = Core::Application;

	public:
		ModelLoading(i32 argc, char** argv);
		~ModelLoading();

	private:
		bool OnInit() override final;
		void OnUpdate(const SDL_Event& e, f32 deltaTime) override final;
		void OnRender() override final;
		void OnResize(u32 width, u32 height) override final;
		void OnShutdown() override final;

	private:
		Graphics::ConstantBuffer<FrameData> m_constantBuffer;
		Graphics::Mesh                      m_mesh;
		Graphics::Material                  m_material;
		Graphics::Camera                    m_camera;
	};
}
