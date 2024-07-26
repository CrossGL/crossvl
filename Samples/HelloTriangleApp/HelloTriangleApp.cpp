#include "HelloTriangleApp.h"
#include <Core/Graphics/Renderer.h>

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(HelloTriangleApp);
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

		m_renderer->SetClearColor(0.0f, 1.0f, 1.0f);

		CGL_LOG(HelloTriangleApp, Info, "Initialized HelloTriangle App");

		return true;
	}

	void HelloTriangleApp::OnUpdate()
	{
	}

	void HelloTriangleApp::OnRender()
	{
	}

	void HelloTriangleApp::OnResize(u32 width, u32 height)
	{
		Super::OnResize(width, height);
	}

	void HelloTriangleApp::OnShutdown()
	{
		CGL_LOG(HelloTriangleApp, Info, "Shutting down HelloTriangle App");
		Super::OnShutdown();
	}
}
