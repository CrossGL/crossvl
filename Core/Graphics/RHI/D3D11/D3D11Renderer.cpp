#include <Core/Graphics/RHI/D3D11/D3D11RendererImpl.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/Shader/ShaderCompiler.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace CGL::Graphics
{
#if defined(CGL_RHI_DX11)

    namespace
    {
        template <typename T, UINT TNameLength>
        inline void SetDebugObjectName([[maybe_unused]] _In_ T* resource,
                                       [[maybe_unused]] _In_z_ const char (&name)[TNameLength])
        {
    #ifdef CGL_BUILD_DEBUG
            if (resource)
            {
                resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
            }
    #endif  // CGL_BUILD_DEBUG
        }

    }  // namespace

    namespace Mapping
    {
        static constexpr std::array<D3D_PRIMITIVE_TOPOLOGY, 5> PrimitiveTopology = {
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,  D3D_PRIMITIVE_TOPOLOGY_LINELIST,  D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
        };

        static constexpr std::array<D3D11_USAGE, 4> BufferUsage = { D3D11_USAGE_DEFAULT, D3D11_USAGE_IMMUTABLE,
                                                                    D3D11_USAGE_DYNAMIC, D3D11_USAGE_STAGING };

        static_assert(PrimitiveTopology.size() == size_t(PrimitiveType::COUNT));
        static_assert(BufferUsage.size() == size_t(BufferUsage::COUNT));
    }  // namespace Mapping

    void Renderer::Constructor_D3D11(SDL_Window* window)
    {
        // Get Win32 window handle from SDL_Window
        SDL_SysWMinfo systemInfo;
        SDL_VERSION(&systemInfo.version);
        SDL_GetWindowWMInfo(window, &systemInfo);
        HWND handle = systemInfo.info.win.window;

        m_impl = new D3D11RendererImpl(handle);

        CGL_LOG(Renderer, Info, "D3D11 Renderer Initialized");
    }

    void Renderer::Destructor_D3D11()
    {
        delete static_cast<D3D11RendererImpl*>(m_impl);
        m_impl = nullptr;

        CGL_LOG(Renderer, Info, "D3D11 Renderer Destroyed");
    }

    D3D11RendererImpl* Renderer::GetImpl() const
    {
        return static_cast<D3D11RendererImpl*>(m_impl);
    }

    void Renderer::BeginFrame_D3D11()
    {
        const auto impl = GetImpl();
        assert(impl && impl->GetContext());

        auto renderTarget = impl->GetBackBuffer();
        impl->GetContext()->RSSetViewports(1, &impl->GetViewport());
        impl->GetContext()->OMSetRenderTargets(1, &renderTarget, nullptr);
        impl->GetContext()->ClearRenderTargetView(renderTarget, m_clearColor.data());
    }

    void Renderer::EndFrame_D3D11()
    {
        const auto impl = GetImpl();
        assert(impl && impl->GetSwapChain());

        impl->GetSwapChain()->Present(0, 0);
    }

    void Renderer::Resize_D3D11(u32 width, u32 height)
    {
        const auto impl = GetImpl();
        assert(impl && impl->GetSwapChain());

        // Clear and flush context
        impl->GetContext()->ClearState();
        impl->GetContext()->Flush();

        impl->ReleaseSizeDependentResources();
        impl->CreateSizeDependentResources(width, height);
    }

    void Renderer::SetPrimitiveTopology_D3D11(PrimitiveType topology)
    {
        const auto impl = GetImpl();
        assert(impl && impl->GetContext());

        impl->GetContext()->IASetPrimitiveTopology(Mapping::PrimitiveTopology[size_t(topology)]);
    }

    void Renderer::SetVertexShader_D3D11(const VertexShader& shader)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->VSSetShader(shader.Shader.Get(), nullptr, 0);
        GetImpl()->GetContext()->IASetInputLayout(shader.InputLayout.Get());
    }

    void Renderer::SetPixelShader_D3D11(const PixelShader& shader)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->PSSetShader(shader.Shader.Get(), nullptr, 0);
    }

    void Renderer::SetVertexBuffer_D3D11(const VertexBuffer& buffer)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->IASetVertexBuffers(0, 1, buffer.Buffer.GetAddressOf(), &buffer.Stride, &buffer.Offset);
    }

    void Renderer::SetIndexBuffer_D3D11(const IndexBuffer& buffer)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->IASetIndexBuffer(buffer.Buffer.Get(), buffer.Format, 0);
    }

    ShaderCompileResult Renderer::CompileVertexShader_D3D11(const ShaderSource& source, VertexShader* outShader)
    {
        assert(GetImpl() && GetImpl()->GetDevice() && outShader);

        CompileConfig cfg{};
        cfg.Target     = "vs_5_0";
        cfg.EntryPoint = "main";
    #ifdef CGL_BUILD_DEBUG
        cfg.Debug    = true;
        cfg.Optimize = false;
    #endif

        ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->Blob);

        if (result.Status != ShaderCompileStatus::Failure)
        {
            assert(outShader->Blob);

            HRESULT hr{ S_OK };
            // Shader compiled with warnings, try creating the shader
            DXCall(hr = GetImpl()->GetDevice()->CreateVertexShader(outShader->Blob->GetBufferPointer(),
                                                                   outShader->Blob->GetBufferSize(), nullptr,
                                                                   &outShader->Shader));

    #ifdef CGL_BUILD_DEBUG
            {
                char name[64];
                sprintf_s(name, "[VS] %s", source.Name.c_str());
                SetDebugObjectName(outShader->Shader.Get(), name);
            }
    #endif  // CGL_BUILD_DEBUG

            // Create reflection
            ComPtr<ID3D11ShaderReflection> vsReflection = nullptr;

            DXCall(hr = D3DReflect(outShader->Blob->GetBufferPointer(), outShader->Blob->GetBufferSize(),
                                   IID_PPV_ARGS(&vsReflection)));

            D3D11_SHADER_DESC desc{};
            DXCall(hr = vsReflection->GetDesc(&desc));

            // Create input layout
            std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
            for (u32 i = 0; i < desc.InputParameters; i++)
            {
                // Get input parameter at index
                D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
                ZeroMemory(&paramDesc, sizeof(paramDesc));
                DXCall(hr = vsReflection->GetInputParameterDesc(i, &paramDesc));

                // Create input element description
                D3D11_INPUT_ELEMENT_DESC elementDesc;
                ZeroMemory(&elementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

                elementDesc.SemanticName         = paramDesc.SemanticName;
                elementDesc.SemanticIndex        = paramDesc.SemanticIndex;
                elementDesc.InputSlot            = 0;
                elementDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
                elementDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
                elementDesc.InstanceDataStepRate = 0;

                // Determine DXGI format
                if (paramDesc.Mask == 1)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                        elementDesc.Format = DXGI_FORMAT_R32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                        elementDesc.Format = DXGI_FORMAT_R32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                        elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                }
                else if (paramDesc.Mask <= 3)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                }
                else if (paramDesc.Mask <= 7)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                else if (paramDesc.Mask <= 15)
                {
                    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                        elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                }

                inputLayout.push_back(elementDesc);
            }

            DXCall(hr = GetImpl()->GetDevice()->CreateInputLayout(
                       inputLayout.data(), (u32)inputLayout.size(), outShader->Blob->GetBufferPointer(),
                       outShader->Blob->GetBufferSize(), &outShader->InputLayout));

    #ifdef CGL_BUILD_DEBUG
            {
                char name[64];
                sprintf_s(name, "[VS ~ Input Layout] %s", source.Name.c_str());
                SetDebugObjectName(outShader->Shader.Get(), name);
            }
    #endif  // CGL_BUILD_DEBUG
        }

        return result;
    }

    ShaderCompileResult Renderer::CompilePixelShader_D3D11(const ShaderSource& source, PixelShader* outShader)
    {
        assert(GetImpl() && GetImpl()->GetDevice() && outShader);

        CompileConfig cfg{};
        cfg.Target     = "ps_5_0";
        cfg.EntryPoint = "main";
    #ifdef CGL_BUILD_DEBUG
        cfg.Debug    = true;
        cfg.Optimize = false;
    #endif

        ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->Blob);

        if (result.Status != ShaderCompileStatus::Failure)
        {
            HRESULT hr{ S_OK };
            // Shader compiled with warnings, try creating the shader
            DXCall(hr = GetImpl()->GetDevice()->CreatePixelShader(outShader->Blob->GetBufferPointer(),
                                                                  outShader->Blob->GetBufferSize(), nullptr,
                                                                  &outShader->Shader));

    #ifdef CGL_BUILD_DEBUG
            {
                char name[64];
                sprintf_s(name, "[PS] %s", source.Name.c_str());
                SetDebugObjectName(outShader->Shader.Get(), name);
            }
    #endif  // CGL_BUILD_DEBUG
        }

        return result;
    }

    VertexBuffer Renderer::CreateVertexBuffer_D3D11(const BufferSource& source)
    {
        assert(source.Type == BufferType::Vertex);
        assert(GetImpl() && GetImpl()->GetDevice());

        CD3D11_BUFFER_DESC desc{};
        desc.Usage          = Mapping::BufferUsage[size_t(source.Usage)];
        desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth      = source.TypeSize * source.Count;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA resourceData{};
        resourceData.pSysMem          = source.Data;
        resourceData.SysMemPitch      = 0;
        resourceData.SysMemSlicePitch = 0;

        ID3D11Buffer* buffer = nullptr;
        DXCall(GetImpl()->GetDevice()->CreateBuffer(&desc, &resourceData, &buffer));

        CGL_LOG(Renderer, Trace, "D3D11 Vertex Buffer Created");

        VertexBuffer vb;
        vb.Buffer.Attach(buffer);
        vb.Stride = source.TypeSize;
        vb.Offset = 0;

        SetDebugObjectName(vb.Buffer.Get(), "D3D11VertexBuffer");

        return vb;
    }

    IndexBuffer Renderer::CreateIndexBuffer_D3D11(const BufferSource& source)
    {
        assert(source.Type == BufferType::Index);
        assert(GetImpl() && GetImpl()->GetDevice());

        CD3D11_BUFFER_DESC desc{};
        desc.Usage          = Mapping::BufferUsage[size_t(source.Usage)];
        desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
        desc.ByteWidth      = source.TypeSize * source.Count;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA resourceData{};
        resourceData.pSysMem          = source.Data;
        resourceData.SysMemPitch      = 0;
        resourceData.SysMemSlicePitch = 0;

        ID3D11Buffer* buffer = nullptr;
        DXCall(GetImpl()->GetDevice()->CreateBuffer(&desc, &resourceData, &buffer));

        CGL_LOG(Renderer, Trace, "D3D11 Index Buffer Created");

        IndexBuffer ib;
        ib.Buffer.Attach(buffer);
        ib.IndicesCount = source.Count;
        ib.Format       = (source.TypeSize == sizeof(u16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);

        SetDebugObjectName(ib.Buffer.Get(), "D3D11IndexBuffer");

        return ib;
    }

    void Renderer::CreateContantBuffer_D3D11(const BufferSource& source, ComPtr<ID3D11Buffer>& outBuffer)
    {
        assert(source.Type == BufferType::Constant);
        assert(GetImpl() && GetImpl()->GetDevice());

        CD3D11_BUFFER_DESC desc{};
        desc.Usage          = Mapping::BufferUsage[size_t(source.Usage)];
        desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        desc.ByteWidth      = source.TypeSize;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        DXCall(GetImpl()->GetDevice()->CreateBuffer(&desc, nullptr, &outBuffer));

        SetDebugObjectName(outBuffer.Get(), "D3D11ConstantBuffer");
    }

    void Renderer::SetConstantBufferData_D3D11(ID3D11Buffer* buffer, const void* data, size_t size)
    {
        assert(buffer && GetImpl() && GetImpl()->GetContext());

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if (SUCCEEDED(GetImpl()->GetContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
        {
            // Ensure the data size fits within the mapped resource
            assert(size <= mappedResource.RowPitch);  // RowPitch is used for 1D and 2D data
            memcpy(mappedResource.pData, data, size);

            GetImpl()->GetContext()->Unmap(buffer, 0);
        }
        else
        {
            CGL_LOG(Renderer, Error, "Failed to map constant buffer data");
        }
    }

    void Renderer::SetContantBuffer_D3D11(ShaderType type, u32 startSlot, const ComPtr<ID3D11Buffer>& buffer)
    {
        assert(GetImpl() && GetImpl()->GetContext() && buffer);

        switch (type)
        {
        case CGL::Graphics::ShaderType::Vertex:
            GetImpl()->GetContext()->VSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;
        case CGL::Graphics::ShaderType::Hull:
            GetImpl()->GetContext()->HSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;
        case CGL::Graphics::ShaderType::Domain:
            GetImpl()->GetContext()->DSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;
        case CGL::Graphics::ShaderType::Geometry:
            GetImpl()->GetContext()->GSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;
        case CGL::Graphics::ShaderType::Pixel:
            GetImpl()->GetContext()->PSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;
        case CGL::Graphics::ShaderType::Compute:
            GetImpl()->GetContext()->CSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
            break;

        default: CGL_LOG(Renderer, Error, "Unable set contant buffer for input shader type"); break;
        }
    }

    void Renderer::Draw_D3D11(u32 vertexCount, u32 startVertex)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->Draw(vertexCount, startVertex);
    }

    void Renderer::DrawIndexed_D3D11(u32 indexCount, u32 startIndex, u32 baseVertex)
    {
        assert(GetImpl() && GetImpl()->GetContext());
        GetImpl()->GetContext()->DrawIndexed(indexCount, startIndex, baseVertex);
    }

#endif  // CGL_RHI_DX11
}  // namespace CGL::Graphics
