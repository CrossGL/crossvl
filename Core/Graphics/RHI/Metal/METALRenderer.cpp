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

        const auto& rCommandEncoder = GetImpl()->GetRenderCommandEncoder();

        rCommandEncoder->setFrontFacingWinding(MTL::WindingClockwise);
        rCommandEncoder->setCullMode(MTL::CullModeBack);
        rCommandEncoder->setTriangleFillMode(MTL::TriangleFillMode::TriangleFillModeFill);
        rCommandEncoder->setRenderPipelineState(GetImpl()->GetRenderPipelineHandler()->GetRenderPipelineState());
        rCommandEncoder->setDepthStencilState(GetImpl()->GetRenderPipelineHandler()->GetDepthStencilState());
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
        GetImpl()->GetRenderPipelineHandler()->SetIndexBuffer(&const_cast<IndexBuffer&>(buffer));
    }

	void Renderer::SetConstantBufferData_METAL(const MTL::Buffer* buffer, const void* data, size_t size)
	{
	    memcpy(const_cast<MTL::Buffer*>(buffer)->contents(), data, size);
	}

	void Renderer::SetConstantBuffer_METAL([[maybe_unused]] ShaderType type, [[maybe_unused]] u32 startSlot, const MTL::Buffer* buffer)
	{
	    GetImpl()->GetRenderCommandEncoder()->setVertexBuffer(buffer, 0, 1);
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
        assert(GetImpl() && GetImpl()->GetDevice());

        const u32 sourceSZ = source.TypeSize * source.Count;

        return IndexBuffer {
            .Buffer = GetImpl()->GetDevice()->newBuffer(source.Data, sourceSZ, MTL::ResourceStorageModeShared),
            .Format = (source.TypeSize == sizeof(u16) ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32),
            .IndicesCount = source.Count
        };
    }

    void Renderer::CreateConstantBuffer_METAL(const BufferSource& source, MTL::Buffer** outBuffer)
    {
        *outBuffer = GetImpl()->GetDevice()->newBuffer(source.TypeSize, MTL::ResourceStorageModeShared);
    }

    void Renderer::Draw_METAL(u32 vertexCount, u32 startVertex)
    {
        GetImpl()->GetRenderCommandEncoder()->drawPrimitives(
            GetImpl()->GetPrimitiveType(), startVertex, vertexCount
        );
    }

    void Renderer::DrawIndexed_METAL(u32 indexCount, [[maybe_unused]] u32 startIndex, [[maybe_unused]] u32 baseVertex)
    {
        const METALIndexBuffer* IndexBuffer = GetImpl()->GetRenderPipelineHandler()->GetIndexBuffer();

        GetImpl()->GetRenderCommandEncoder()->drawIndexedPrimitives(
            GetImpl()->GetPrimitiveType(), indexCount, IndexBuffer->Format, IndexBuffer->Buffer, 0
        );
    }
}
