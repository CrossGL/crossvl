#include "ModelLoading.h"
#include <Core/Application/AssetFinder.h>

#if defined(CGL_RHI_OPENGL) || defined(CGL_RHI_VULKAN)
	#define CGL_UPLOAD_MATRIX(mat) mat
#else
	#define CGL_UPLOAD_MATRIX(mat) mat.Transpose()
#endif

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(ModelLoading);

	static constexpr byte s_vertexShader[] =
	{
#if defined(CGL_RHI_DX11)
		#include "ModelLoadingVS.hlsl.h"	
#elif defined(CGL_RHI_OPENGL)
		#include "ModelLoadingVS.vert.h"
#endif
	};

	static constexpr byte s_pixelShader[] =
	{
#if defined(CGL_RHI_DX11)
		#include "ModelLoadingPS.hlsl.h"	
#elif defined(CGL_RHI_OPENGL)
		#include "ModelLoadingPS.frag.h"
#endif	
	};

	ModelLoading::ModelLoading(i32 argc, char** argv) 
		: Super("[CGL] ModelLoading", argc, argv)
	{
		CGL_LOG(ModelLoading, Trace, "Created ModelLoading");
	}

	ModelLoading::~ModelLoading()
	{
		CGL_LOG(ModelLoading, Trace, "Destroyed ModelLoading");
	}

	bool ModelLoading::OnInit()
	{
		if (!Super::OnInit())
			return false;

		SM::Color color = Colors::Chocolate.v;
		GetRenderer()->SetClearColor(color.R(), color.G(), color.B(), color.A());

		// Define vertex shader source
		Graphics::ShaderSource vsSrc;
		vsSrc.SourceData = Core::DataToString(s_vertexShader, sizeof(s_vertexShader));
		vsSrc.Type       = Graphics::ShaderType::Vertex;
		vsSrc.Name       = "ModelLoadingVS";
		vsSrc.Name       = "ModelLoadingVS";

		// Define pixel shader source
		Graphics::ShaderSource psSrc;
		psSrc.SourceData = Core::DataToString(s_pixelShader, sizeof(s_pixelShader));
		psSrc.Type       = Graphics::ShaderType::Pixel;
		psSrc.Name       = "ModelLoadingPS";

		// Compile material
		m_material.AddSource(vsSrc);
		m_material.AddSource(psSrc);
		if (!GetRenderer()->CompileMaterial(&m_material))
		{
			CGL_LOG(ModelLoading, Error, "Failed to compile material for triangle");
		}

		// Load Mesh
		if (!m_mesh.LoadOBJ("Models/Suzanne.obj"))
		{
			CGL_LOG(ModelLoading, Error, "Failed to load Suzanne.obj");
			assert(false);
		}

		// Create mesh buffers
		for (Graphics::SubMesh& subMesh: m_mesh.SubMeshes)
		{
			auto& meshData = subMesh.GetMeshData();
			const u32 vertexCount = static_cast<u32>(subMesh.GetVertices().size());

			// Create vertex buffer
			const auto& indices  = subMesh.GetIndices();
			const auto& vertices = subMesh.GetVertices();
			const auto& normals  = subMesh.GetNormals();
			const auto& uvs      = subMesh.GetUVs();

			std::vector<Graphics::VertexTypes::PositionNormalTexture> vb;
			for (u32 i = 0; i < vertexCount; i++)
			{
				Graphics::VertexTypes::PositionNormalTexture vertex;
				vertex.Position = SM::Vector4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);
				vertex.Normal   = normals[i];
				vertex.Texture  = uvs[i];

				vb.push_back(vertex);
			}

			Graphics::BufferSource vbs;
			vbs.Data              = (void*)vb.data();
			vbs.Type              = Graphics::BufferType::Vertex;
			vbs.TypeSize          = sizeof(decltype(vb)::value_type);
			vbs.Count             = u32(vb.size());
			vbs.VertexType = typeid(decltype(vb)::value_type);
			meshData.VBuffer = GetRenderer()->CreateVertexBuffer(vbs);

			// Create index buffer
			Graphics::BufferSource ibs;			
			ibs.Data             = (void*)indices.data();
			ibs.Type             = Graphics::BufferType::Index;
			ibs.TypeSize         = sizeof(u32);
			ibs.Count            = u32(indices.size());
			meshData.IBuffer = GetRenderer()->CreateIndexBuffer(ibs);
		}

		// Create constant buffer
		Graphics::BufferSource cbs;
		cbs.Type     = Graphics::BufferType::Constant;
		cbs.Usage    = Graphics::BufferUsage::Dynamic;
		cbs.TypeSize = sizeof(decltype(m_constantBuffer)::value_type);
		GetRenderer()->CreateContantBuffer(cbs, m_constantBuffer);

		// Create camera
		m_camera.InitAsPerspective(90, f32(GetRenderer()->GetWidth()), f32(GetRenderer()->GetHeight()));
		m_camera.SetPosition({ 0.0f, 0.5f, -2.5f });

		CGL_LOG(ModelLoading, Info, "Initialized ModelLoading");

		return true;
	}

	f32 time = 0.0f;
	void ModelLoading::OnUpdate([[maybe_unused]] const SDL_Event& e, f32 deltaTime)
	{
		time += deltaTime;
		FrameData data
		{
			.World = CGL_UPLOAD_MATRIX(SM::Matrix::CreateRotationY(time).Transpose()),
			.View = CGL_UPLOAD_MATRIX(m_camera.GetViewMatrix()),
			.Projection = CGL_UPLOAD_MATRIX(m_camera.GetProjectionMatrix())
		};

		GetRenderer()->SetConstantBufferData(m_constantBuffer, data);
	}

	void ModelLoading::OnRender()
	{
		GetRenderer()->SetPrimitiveTopology(Graphics::PrimitiveType::TriangleList);
		GetRenderer()->SetMaterial(m_material);
		GetRenderer()->SetConstantBuffer(Graphics::ShaderType::Vertex, 0, m_constantBuffer);
		
		for (auto& subMesh : m_mesh.SubMeshes)
		{
			auto& meshData = subMesh.GetMeshData();
			GetRenderer()->SetVertexBuffer(meshData.VBuffer);
			GetRenderer()->SetIndexBuffer(meshData.IBuffer);

			GetRenderer()->DrawIndexed(meshData.IBuffer.IndicesCount);
		}
	}

	void ModelLoading::OnResize(u32 width, u32 height)
	{
		m_camera.OnResize(width, height);
		Super::OnResize(width, height);
	}

	void ModelLoading::OnShutdown()
	{
		CGL_LOG(ModelLoading, Info, "Shutting down ModelLoading");
		Super::OnShutdown();
	}
}
