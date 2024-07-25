#include "HelloTriangleApp.h"
#include <Core/Graphics/Renderer.h>

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(HelloTriangleApp);

	Graphics::Renderer* renderer = nullptr;

	HelloTriangleApp::HelloTriangleApp()
	{
		CGL_LOG(HelloTriangleApp, Trace, "Created HelloTriangle App");
	}
	HelloTriangleApp::~HelloTriangleApp()
	{
		CGL_LOG(HelloTriangleApp, Trace, "Destroyed HelloTriangle App");
	}

	bool HelloTriangleApp::OnInit()
	{
		if (!Super::OnInit())
			return false;

		renderer = new Graphics::Renderer(m_window, Graphics::RHIType::DirectX11);

		CGL_LOG(HelloTriangleApp, Info, "Initialized HelloTriangle App");

		return true;
	}

	void HelloTriangleApp::OnUpdate()
	{
		Super::OnUpdate();
	}

	void HelloTriangleApp::OnRender()
	{
		Super::OnRender();
	}

	void HelloTriangleApp::OnResize(u32 width, u32 height)
	{
		CGL_LOG(HelloTriangleApp, Info, "Resized HelloTriangle App {}x{}", width, height);
		Super::OnResize(width, height);
	}

	void HelloTriangleApp::OnShutdown()
	{
		if (renderer)
		{
			delete renderer;
			renderer = nullptr;
		}
		CGL_LOG(HelloTriangleApp, Info, "Shutting down HelloTriangle App");
		Super::OnShutdown();
	}
}
