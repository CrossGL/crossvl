#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/RHI/D3D11/D3D11RendererImpl.h>
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
#endif // CGL_RHI_DX11
}