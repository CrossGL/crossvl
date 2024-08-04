#include "HelloTriangle.h"
#include <Core/Application/AssetFinder.h>

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(HelloTriangle);

	static constexpr byte s_vertexShader[] =
	{
		#include "HelloTriangleVS.hlsl.h"
	};

	static constexpr byte s_pixelShader[] =
	{
		#include "HelloTrianglePS.hlsl.h"
	};

	HelloTriangle::HelloTriangle(i32 argc, char** argv) 
		: Super("[CGL] Hello Triangle Sample", argc, argv)
	{
		CGL_LOG(HelloTriangle, Trace, "Created HelloTriangle App");
	}
	HelloTriangle::~HelloTriangle()
	{
		CGL_LOG(HelloTriangle, Trace, "Destroyed HelloTriangle App");
	}

	bool HelloTriangle::OnInit()
	{
		if (!Super::OnInit())
			return false;

		GetRenderer()->SetClearColor(0.0f, 1.0f, 1.0f);

		// Define vertex shader source
		Graphics::ShaderSource vsSrc;
		vsSrc.SourceData = Core::DataToString(s_vertexShader, sizeof(s_vertexShader));
		vsSrc.Type       = Graphics::ShaderType::Vertex;
		vsSrc.Name       = "HelloTriangleVS";

		// Define pixel shader source
		Graphics::ShaderSource psSrc;
		psSrc.SourceData = Core::DataToString(s_pixelShader, sizeof(s_pixelShader));
		psSrc.Type = Graphics::ShaderType::Pixel;
		psSrc.Name = "HelloTrianglePS";
		
		// Compile material
		m_material.AddSource(vsSrc);
		m_material.AddSource(psSrc);
		if (!GetRenderer()->CompileMaterial(&m_material))
		{
			CGL_LOG(HelloTriangle, Error, "Failed to compile material for triangle");
		}

		 // Define triangle vertices
		constexpr std::array vertices =
		{
			Graphics::VertexTypes::PositionColor
			{
				.Position = SM::Vector3(  0.0f,  0.5f, 0.0f ),
				.Color    = SM::Vector4( 1.0f, 0.0f, 0.0f, 1.0f ),
			},
			Graphics::VertexTypes::PositionColor
			{
				.Position = SM::Vector3(  0.5f, -0.5f, 0.0f ),
				.Color    = SM::Vector4( 0.0f, 1.0f, 0.0f, 1.0f ),
			},
			Graphics::VertexTypes::PositionColor
			{
				.Position = SM::Vector3( -0.5f, -0.5f, 0.0f ),
				.Color    = SM::Vector4( 0.0f, 0.0f, 1.0f, 1.0f ),
			},
		};

		Graphics::BufferSource vbs;
		vbs.Data       = (void*)vertices.data();
		vbs.Type       = Graphics::BufferType::Vertex;
		vbs.TypeSize   = sizeof(decltype(vertices)::value_type);
		vbs.Count      = u32(vertices.size());
		m_vertexBuffer = GetRenderer()->CreateVertexBuffer(vbs);

		CGL_LOG(HelloTriangle, Info, "Initialized HelloTriangle App");
		return true;
	}

	void HelloTriangle::OnUpdate([[maybe_unused]] const SDL_Event& e)
	{
	}

	void HelloTriangle::OnRender()
	{
		GetRenderer()->SetPrimitiveTopology(Graphics::PrimitiveType::TriangleList);
		GetRenderer()->SetMaterial(m_material);
		GetRenderer()->SetVertexBuffer(m_vertexBuffer);
		GetRenderer()->Draw(3, 0);
	}

	void HelloTriangle::OnResize(u32 width, u32 height)
	{
		Super::OnResize(width, height);
	}

	void HelloTriangle::OnShutdown()
	{
		CGL_LOG(HelloTriangle, Info, "Shutting down HelloTriangle App");

		Super::OnShutdown();
	}
}
