#include "SpinningCube.h"
#include <Core/Application/AssetFinder.h>
namespace CGL
{
    CGL_DEFINE_LOG_CATEGORY(SpinningCube);

    static constexpr byte s_vertexShader[] =
    {
  #if defined(CGL_RHI_DX11)
        #include "SpinningCubeVS.hlsl.h"		
  #elif defined(CGL_RHI_OPENGL)
        #include "SpinningCubeVS.vert.h"
  #elif defined(CGL_RHI_METAL)
        #include "SpinningCubeVS.metal.h"
  #endif		
    };

    static constexpr byte s_pixelShader[] =
    {
  #if defined(CGL_RHI_DX11)
        #include "SpinningCubePS.hlsl.h"		
  #elif defined(CGL_RHI_OPENGL)
        #include "SpinningCubePS.frag.h"
  #elif defined(CGL_RHI_METAL)
        #include "SpinningCubePS.metal.h"
  #endif
    };

    SpinningCube::SpinningCube(i32 argc, char** argv) 
        : Super("[CGL] Spinning Cube Sample", argc, argv)
    {
        CGL_LOG(SpinningCube, Trace, "Created SpinningCube");
    }

    SpinningCube::~SpinningCube()
    {
        CGL_LOG(SpinningCube, Trace, "Destroyed SpinningCube");
    }

    bool SpinningCube::OnInit()
    {
        if (!Super::OnInit())
            return false;

        SM::Color color = Colors::BlanchedAlmond.v;
        GetRenderer()->SetClearColor(color.R(), color.G(), color.B(), color.A());

        // Define vertex shader source
        Graphics::ShaderSource vsSrc;
        vsSrc.SourceData = Core::DataToString(s_vertexShader, sizeof(s_vertexShader));
        vsSrc.Type       = Graphics::ShaderType::Vertex;
        vsSrc.Name       = "SpinningCubeVS";

        // Define pixel shader source
        Graphics::ShaderSource psSrc;
        psSrc.SourceData = Core::DataToString(s_pixelShader, sizeof(s_pixelShader));
        psSrc.Type       = Graphics::ShaderType::Pixel;
        psSrc.Name       = "SpinningCubePS";

        // Compile material
        m_material.AddSource(vsSrc);
        m_material.AddSource(psSrc);
        if (!GetRenderer()->CompileMaterial(&m_material))
        {
            CGL_LOG(SpinningCube, Error, "Failed to compile material for triangle");
        }

        // Define cube vertices
        constexpr std::array<Graphics::VertexTypes::PositionColor, 8> vertices =
        {
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(-1.0f, 1.0f, -1.0f),
                .Color = SM::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(1.0f, 1.0f, -1.0f),
                .Color = SM::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(1.0f, 1.0f, 1.0f),
                .Color = SM::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(-1.0f, 1.0f, 1.0f),
                .Color = SM::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(-1.0f, -1.0f, -1.0f),
                .Color = SM::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(1.0f, -1.0f, -1.0f),
                .Color = SM::Vector4(0.0f, 1.0f, 1.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(1.0f, -1.0f, 1.0f),
                .Color = SM::Vector4(1.0f, 0.5f, 0.0f, 1.0f),
            },
            Graphics::VertexTypes::PositionColor
            {
                .Position = SM::Vector3(-1.0f, -1.0f, 1.0f),
                .Color = SM::Vector4(0.5f, 0.0f, 0.5f, 1.0f),
            },
        };

        Graphics::BufferSource vbs;
        vbs.Data       = (void*)vertices.data();
        vbs.Type       = Graphics::BufferType::Vertex;
        vbs.TypeSize   = sizeof(decltype(vertices)::value_type);
        vbs.Count      = u32(vertices.size());
        m_vertexBuffer = GetRenderer()->CreateVertexBuffer(vbs);

        // Define cube indices
        constexpr std::array<u16, 36> indices =
        {
          3,1,0,
          2,1,3,

          0,5,4,
          1,5,0,

          3,4,7,
          0,4,3,

          1,6,5,
          2,6,1,

          2,7,6,
          3,7,2,

          6,4,5,
          7,4,6,
        };

        Graphics::BufferSource ibs;
        ibs.Data       = (void*)indices.data();
        ibs.Type       = Graphics::BufferType::Index;
        ibs.TypeSize   = sizeof(decltype(indices)::value_type);
        ibs.Count      = u32(indices.size());
        m_indexBuffer  = GetRenderer()->CreateIndexBuffer(ibs);

        // Create constant buffer
        Graphics::BufferSource cbs;
        cbs.Type     = Graphics::BufferType::Constant;
        cbs.Usage    = Graphics::BufferUsage::Dynamic;
        cbs.TypeSize = sizeof(decltype(m_constantBuffer)::value_type);
        GetRenderer()->CreateContantBuffer(cbs, m_constantBuffer);

        // Create camera
        m_camera.InitAsPerspective(90, f32(GetRenderer()->GetWidth()), f32(GetRenderer()->GetHeight()));
        m_camera.SetPosition({0.0f, 1.5f, -2.5f});
        CGL_LOG(SpinningCube, Info, "Initialized SpinningCube");
        return true;
    }

    f32 time = 0.0f;
    void SpinningCube::OnUpdate([[maybe_unused]] const SDL_Event& e)
    {
        time += 0.0001f;
        FrameData data
        {
            .World      = SM::Matrix::CreateRotationY(time),
            .View       = m_camera.GetViewMatrix().Transpose(),
            .Projection = m_camera.GetProjectionMatrix().Transpose()
        };

        GetRenderer()->SetConstantBufferData(m_constantBuffer, data);
    }

    void SpinningCube::OnRender()
    {
        GetRenderer()->SetPrimitiveTopology(Graphics::PrimitiveType::TriangleList);
        GetRenderer()->SetMaterial(m_material);
        GetRenderer()->SetContantBuffer(Graphics::ShaderType::Vertex, 0, m_constantBuffer);
        GetRenderer()->SetVertexBuffer(m_vertexBuffer);
        GetRenderer()->SetIndexBuffer(m_indexBuffer);

        GetRenderer()->DrawIndexed(m_indexBuffer.IndicesCount);
    }

    void SpinningCube::OnResize(u32 width, u32 height)
    {
        m_camera.OnResize(width, height);
        Super::OnResize(width, height);
    }

    void SpinningCube::OnShutdown()
    {
        CGL_LOG(SpinningCube, Info, "Shutting down SpinningCube");
        Super::OnShutdown();
    }
}