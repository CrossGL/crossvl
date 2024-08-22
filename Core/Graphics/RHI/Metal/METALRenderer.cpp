#include "Core/Graphics/Renderer.h"
#include "Core/Graphics/RHI/Metal/METALRendererImpl.h"

namespace CGL::Graphics
{

    void Renderer::Constructor_METAL(SDL_Window* window)
    {
        this->m_impl = new METALRendererImpl(window);

        CGL_LOG(Renderer, Info, "Renderer Initialized");
    }

    void Renderer::Destructor_METAL()
    {
        delete static_cast<METALRendererImpl*>(m_impl);
        m_impl = nullptr;

        CGL_LOG(Renderer, Info, "Renderer Destroyed");
    }

    void Renderer::BeginFrame_METAL()
    {
        assert(GetImpl());

        GetImpl()->InitAutoReleasePool();

        GetImpl()->SetNextDrawable();
        GetImpl()->SetCommandBuffer();

        GetImpl()->startEncoding(m_clearColor);
    }

    void Renderer::EndFrame_METAL()
    {
        assert(GetImpl());

        GetImpl()->GetRenderCommandEncoder()->endEncoding();
        GetImpl()->GetCommandBuffer()->presentDrawable(GetImpl()->GetMetalContext());
        GetImpl()->GetCommandBuffer()->commit();
        GetImpl()->GetAutoReleasePool()->release();
    }

    void Renderer::Resize_METAL(u32 width, u32 height)
    {
        assert(GetImpl());

        GetImpl()->GetMetalLayer()->setDrawableSize(
            CGSize { (f32)width, (f32)height }
        );
    }

    METALRendererImpl* Renderer::GetImpl() const
    {
        return static_cast<METALRendererImpl*>(m_impl);
    }

    void Renderer::SetRenderPipeline_METAL()
    {
        GetImpl()->GetRenderPipelineHandler()->CreateRenderPipelineState(GetImpl()->GetDevice());
        GetImpl()->GetRenderCommandEncoder()->setRenderPipelineState(GetImpl()->GetRenderPipelineHandler()->GetRenderPipelineState());
    }

    void Renderer::SetPrimitiveTopology_METAL(PrimitiveType topology)
    {
        static constexpr std::array<MTL::PrimitiveType, 5> MTLPrimitives = {
            MTL::PrimitiveTypeTriangle,
            MTL::PrimitiveTypeLine,
            MTL::PrimitiveTypePoint,
            MTL::PrimitiveTypeTriangleStrip,
            MTL::PrimitiveTypeLineStrip
        };

        static_assert(MTLPrimitives.size() == (size_t)PrimitiveType::COUNT);

        GetImpl()->SetPrimitiveType(MTLPrimitives[size_t(topology)]);
    }

    void Renderer::SetVertexShader_METAL(const VertexShader& shader)
    {
        const auto rpDescriptor = GetImpl()->GetRenderPipelineHandler()->GetRenderPipelineDescriptor();

        rpDescriptor->setVertexFunction(shader.Shader);
    }

    void Renderer::SetPixelShader_METAL(const PixelShader& shader)
    {
        const auto rpDescriptor = GetImpl()->GetRenderPipelineHandler()->GetRenderPipelineDescriptor();

        rpDescriptor->setFragmentFunction(shader.Shader);
    }

    void Renderer::SetVertexBuffer_METAL(const VertexBuffer& buffer)
    {
        GetImpl()->GetRenderCommandEncoder()->setVertexBuffer(
            buffer.Buffer, buffer.Offset, buffer.Index
        );
    }

    void Renderer::SetIndexBuffer_METAL(const IndexBuffer& buffer)
    {
        // TODO: implement index buffer
    }

    ShaderCompileResult Renderer::CompileVertexShader_METAL(const ShaderSource& source, VertexShader* outShader)
    {
        assert(GetImpl() && GetImpl()->GetDevice() && outShader);

        CompileConfig Config;

    #ifdef CGL_BUILD_DEBUG
        Config.Debug = true;
        Config.Optimize = false;
    #endif

        METALCompileObjects metal_object = {
            .device = GetImpl()->GetDevice(),
            .library = outShader->SourceContent
        };

        ShaderCompileResult result = ShaderCompiler::Compile(source, Config, metal_object);

        outShader->Shader = (*outShader->SourceContent)->newFunction(
            NS::String::string(source.Name.c_str(), NS::StringEncoding::ASCIIStringEncoding)
        );

        return result;
    }

    ShaderCompileResult Renderer::CompilePixelShader_METAL(const ShaderSource& source, PixelShader* outShader)
    {
        assert(GetImpl() && GetImpl()->GetDevice() && outShader);

        CompileConfig Config;

    #ifdef CGL_BUILD_DEBUG
        Config.Debug = true;
        Config.Optimize = false;
    #endif

        METALCompileObjects metal_object = {
            .device = GetImpl()->GetDevice(),
            .library = outShader->SourceContent
        };

        ShaderCompileResult result = ShaderCompiler::Compile(source, Config, metal_object);


        outShader->Shader = (*outShader->SourceContent)->newFunction(
            NS::String::string(source.Name.c_str(), NS::StringEncoding::ASCIIStringEncoding)
        );

        return result;
    }

    VertexBuffer Renderer::CreateVertexBuffer_METAL(const BufferSource& source)
    {
        assert(GetImpl() && GetImpl()->GetDevice());

        const u32 sourceSZ = source.TypeSize * source.Count;

        return VertexBuffer {
            .Buffer = GetImpl()->GetDevice()->newBuffer(source.Data, sourceSZ, MTL::ResourceStorageModeShared),
            .Offset = 0,
            .Index = 0
        };
    }

    IndexBuffer Renderer::CreateIndexBuffer_METAL(const BufferSource& source)
    {
        // TODO: implement Index Buffer
    }

    void Renderer::Draw_METAL(u32 vertexCount, u32 startVertex)
    {
        GetImpl()->GetRenderCommandEncoder()->drawPrimitives(
            GetImpl()->GetPrimitiveType(), startVertex, vertexCount
        );
    }

    void Renderer::DrawIndexed_METAL(u32 indexCount, u32 startIndex, u32 baseVertex)
    {
        // TODO: implement indexed drawcall
    }
}
