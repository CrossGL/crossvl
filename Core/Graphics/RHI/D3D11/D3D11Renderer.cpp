#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/RHI/D3D11/D3D11RendererImpl.h>

namespace CGL::Graphics
{
#ifdef CGL_RHI_DX11
	void Renderer::Constructor_D3D11()
	{
		m_impl = new D3D11RendererImpl();

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

	}

	void Renderer::EndFrame_D3D11()
	{

	}

	void Renderer::Resize_D3D11(u32 width, u32 height)
	{
	}
#endif // CGL_RHI_DX11
}