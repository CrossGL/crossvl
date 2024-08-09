#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>
#include <Core/Graphics/Types.h>

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(D3D11RendererImpl);

    class D3D11RendererImpl
    {
        friend class Renderer;

    public:
        D3D11RendererImpl(HWND window);
        ~D3D11RendererImpl();

        void CreateSizeDependentResources(u32 width, u32 height);
        void ReleaseSizeDependentResources();

        inline ID3D11Device* GetDevice() const { return m_device.Get(); }
        inline ID3D11DeviceContext* GetContext() const { return m_context.Get(); }
        inline IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }
        inline ID3D11RenderTargetView* GetBackBuffer() const { return m_backBuffer.Get(); }
        inline ID3D11Texture2D* GetBackBufferTexture() const { return m_backBufferTexture.Get(); }
        inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

    private:
        void Init();
        void Shutdown();

        void CreateDevice();
        void CreateSwapChain(u32 width, u32 height);
        void ResizeSwapchain(u32 width, u32 height);

    private:
        HWND m_window{ nullptr };
        D3D11_VIEWPORT m_viewport;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_backBuffer;
        ComPtr<ID3D11Texture2D> m_backBufferTexture;
    };
}  // namespace CGL::Graphics
