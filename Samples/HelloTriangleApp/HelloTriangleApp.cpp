#include "HelloTriangleApp.h"
#include <Core/Graphics/Renderer.h>
#include <Core/Application/AssetFinder.h>

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(HelloTriangleApp);

	static constexpr byte s_vertexShader[] =
	{
		#include "HelloTriangleVS.hlsl.h"
	};

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

		GetRenderer()->SetClearColor(0.0f, 1.0f, 1.0f);

		Graphics::ShaderSource vsSrc;
		vsSrc.SourceData = Core::DataToString(s_vertexShader, sizeof(s_vertexShader));
		vsSrc.Type       = Graphics::ShaderType::Vertex;
		vsSrc.Name       = "HelloTriangleVS";
		m_vertexShader   = GetRenderer()->CreateVertexShader(vsSrc);

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
