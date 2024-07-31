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

	static constexpr byte s_pixelShader[] =
	{
		#include "HelloTrianglePS.hlsl.h"
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

		// Create vertex shader
		Graphics::ShaderSource vsSrc;
		vsSrc.SourceData = Core::DataToString(s_vertexShader, sizeof(s_vertexShader));
		vsSrc.Type       = Graphics::ShaderType::Vertex;
		vsSrc.Name       = "HelloTriangleVS";
		m_vertexShader   = GetRenderer()->CreateVertexShader(vsSrc);

		// Create pixel shader
		Graphics::ShaderSource psSrc;
		psSrc.SourceData = Core::DataToString(s_pixelShader, sizeof(s_pixelShader));
		psSrc.Type       = Graphics::ShaderType::Pixel;
		psSrc.Name       = "HelloTrianglePS";
		m_pixelShader    = GetRenderer()->CreatePixelShader(psSrc);

		// Create vertex buffer
		constexpr std::array vertices =
		{
			Graphics::VertexTypes::PositionColor
			{
				.Position = DX::XMFLOAT3(  0.0f,  0.5f, 0.0f ),
				.Color    = DX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ),
			},
			Graphics::VertexTypes::PositionColor
			{
				.Position = DX::XMFLOAT3(  0.5f, -0.5f, 0.0f ),
				.Color    = DX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ),
			},
			Graphics::VertexTypes::PositionColor
			{
				.Position = DX::XMFLOAT3( -0.5f, -0.5f, 0.0f ),
				.Color    = DX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ),
			},
		};

		Graphics::BufferSource vbs;
		vbs.Data       = (void*)vertices.data();
		vbs.Type       = Graphics::BufferType::Vertex;
		vbs.Size       = sizeof(Graphics::VertexTypes::PositionColor);
		vbs.Count      = u32(vertices.size());
		m_vertexBuffer = GetRenderer()->CreateVertexBuffer(vbs);

		CGL_LOG(HelloTriangleApp, Info, "Initialized HelloTriangle App");
		return true;
	}

	void HelloTriangleApp::OnUpdate()
	{
	}

	void HelloTriangleApp::OnRender()
	{
		GetRenderer()->SetPrimitiveTopology(Graphics::PrimitiveType::TriangleList);
		GetRenderer()->SetVertexShader(m_vertexShader);
		GetRenderer()->SetPixelShader(m_pixelShader);
		GetRenderer()->SetVertexBuffer(m_vertexBuffer);
		GetRenderer()->Draw(3, 0);
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
