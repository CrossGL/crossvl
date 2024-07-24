#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>
#include <Core/Graphics/Types.h>

#include <d3d11.h>

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(D3D11RendererImpl);

	class D3D11RendererImpl
	{
		friend class Renderer;
	public:
		D3D11RendererImpl();
		~D3D11RendererImpl();

		inline ID3D11Device* GetDevice() const { return m_device.Get(); }
		inline ID3D11DeviceContext* GetContext() const { return m_context.Get(); }
		inline IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }
		inline ID3D11RenderTargetView* GetBackBuffer() const { return m_backBuffer.Get(); }
		inline ID3D11Texture2D* GetBackBufferTexture() const { return m_backBufferTexture.Get(); }
		inline ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }

	private:
		ComPtr<ID3D11Device>           m_device;
		ComPtr<ID3D11DeviceContext>    m_context;
		ComPtr<IDXGISwapChain>         m_swapChain;
		ComPtr<ID3D11RenderTargetView> m_backBuffer;
		ComPtr<ID3D11Texture2D>        m_backBufferTexture;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	};
}
