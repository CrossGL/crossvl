#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/RHI/D3D11/D3D11RendererImpl.h>
#include <Core/Graphics/Shader/ShaderCompiler.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace CGL::Graphics
{
#ifdef CGL_RHI_DX11
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
		assert(GetAPI() == RHIType::DirectX11);
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
		static constexpr std::array mapping = 
		{ 
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
			D3D_PRIMITIVE_TOPOLOGY_LINELIST, 
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN
		};
		static_assert(mapping.size() == size_t(PrimitiveType::COUNT));

		const auto impl = GetImpl();
		assert(impl && impl->GetContext());

		impl->GetContext()->IASetPrimitiveTopology(mapping[size_t(topology)]);
	}

	ShaderCompileResult Renderer::CreateVertexShader_D3D11(const ShaderSource& source, std::shared_ptr<VertexShader>& outShader)
	{
		assert(GetImpl() && GetImpl()->GetDevice());

		CompileConfig cfg{};
		cfg.Target = "vs_5_0";
		cfg.EntryPoint = "main";
#ifdef CGL_BUILD_DEBUG
		cfg.Debug = true;
		cfg.Optimize = false;
#endif

		ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->m_blob);

		if (result.Status != ShaderCompileStatus::Failure)
		{
			assert(outShader->m_blob);

			HRESULT hr{ S_OK };
			// Shader compiled with warnings, try compiling the shader
			DXCall(hr = GetImpl()->GetDevice()->CreateVertexShader(
				outShader->m_blob->GetBufferPointer(),
				outShader->m_blob->GetBufferSize(),
				nullptr,
				&outShader->m_shader
			));

			// Create reflection
			ComPtr<ID3D11ShaderReflection> vsReflection = nullptr;

			DXCall(hr = D3DReflect(
				outShader->m_blob->GetBufferPointer(),
				outShader->m_blob->GetBufferSize(),
				IID_PPV_ARGS(&vsReflection)
			));

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
				inputLayout.data(),
				(u32)inputLayout.size(),
				outShader->m_blob->GetBufferPointer(),
				outShader->m_blob->GetBufferSize(),
				&outShader->m_layout
			));
		}

		return result;
	}
#endif // CGL_RHI_DX11
}