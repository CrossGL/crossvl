#pragma once
#include <Core/Common.h>
#include <Core/Application/Application.h>

namespace CGL
{
	CGL_DECLARE_LOG_CATEGORY(HelloTriangleApp);

	class HelloTriangleApp : public Core::Application
	{
	protected:
		using Super = Core::Application;

	public:
		HelloTriangleApp();
		~HelloTriangleApp();

	private:
		bool OnInit() override final;
		void OnUpdate() override final;
		void OnRender() override final;
		void OnResize(u32 width, u32 height) override final;
		void OnShutdown() override final;

	private:
		std::shared_ptr<Graphics::VertexShader> m_vertexShader;
		std::shared_ptr<Graphics::PixelShader> m_pixelShader;
		std::shared_ptr<Graphics::VertexBuffer> m_vertexBuffer;
		std::shared_ptr<Graphics::IndexBuffer> m_indexBuffer;
	};
}
