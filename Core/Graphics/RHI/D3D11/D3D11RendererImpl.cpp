#pragma once
#include "D3D11RendererImpl.h"

namespace CGL::Graphics
{

	namespace
	{
		template<typename T, UINT TNameLength>
		inline void SetDebugObjectName([[maybe_unused]]_In_ T* resource, [[maybe_unused]]_In_z_ const char(&name)[TNameLength])
		{
#ifdef CGL_BUILD_DEBUG
			if (resource)
			{
				resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
			}
#endif  // CGL_BUILD_DEBUG
		}

	}


	CGL_DEFINE_LOG_CATEGORY(D3D11RendererImpl);

	D3D11RendererImpl::D3D11RendererImpl(HWND window)
		: m_window(window)
	{
		Init();
	}

	D3D11RendererImpl::~D3D11RendererImpl()
	{
		Shutdown();
	}

	void Graphics::D3D11RendererImpl::CreateSizeDependentResources(u32 width, u32 height)
	{
		// This will resize if needed
		CreateSwapChain(width, height);

		// Update viewport
		m_viewport.Width    = f32(width);
		m_viewport.Height   = f32(height);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;

		HRESULT hr{ S_OK };
		assert(m_swapChain && m_context);

		// Get back buffer
		DXCall(hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBufferTexture)));
		
		// Create render target view
		DXCall(hr = m_device->CreateRenderTargetView(m_backBufferTexture.Get(), nullptr, &m_backBuffer));
	}

	void Graphics::D3D11RendererImpl::ReleaseSizeDependentResources()
	{
		m_backBufferTexture.Reset();
		m_backBuffer.Reset();
	}

	void D3D11RendererImpl::Init()
	{
		RECT rect;
		::GetClientRect(m_window, &rect);
		const u32 width = rect.right - rect.left;
		const u32 height = rect.bottom - rect.top;

		CreateDevice();
		CreateSizeDependentResources(width, height);
	}

	void D3D11RendererImpl::Shutdown()
	{
		m_context->ClearState();
		m_context->Flush();

		m_backBuffer.Reset();
		m_backBufferTexture.Reset();
		m_swapChain.Reset();
		m_context.Reset();
		m_device.Reset();

#ifdef CGL_BUILD_DEBUG
		{
			ComPtr<IDXGIDebug> dxgiDebug;
			if (SUCCEEDED(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
			{
				dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
			}
		}
#endif

		CGL_LOG(D3D11RendererImpl, Trace, "D3D11 Renderer Shutdown");
	}

	void D3D11RendererImpl::CreateDevice()
	{
		// Device only needs to be created once
		if (!m_device)
		{
			HRESULT hr{ S_OK };
			u32 flags = 0;
#ifdef CGL_BUILD_DEBUG
			CGL_LOG(D3D11RendererImpl, Debug, "Enabling debug layer for D3D11 device");
			flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			DXCall(hr = ::D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				flags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&m_device,
				nullptr,
				&m_context
			));

			assert(m_device && m_context);

			SetDebugObjectName(m_device.Get(), "D3D11Device");
			SetDebugObjectName(m_context.Get(), "D3D11DeviceContext");

			// Create debug interface
#ifdef CGL_BUILD_DEBUG
			{
				ComPtr<ID3D11Debug> debug = nullptr;
				if (SUCCEEDED(m_device.As(&debug)))
				{
					ComPtr<ID3D11InfoQueue> infoQueue = nullptr;
					if (SUCCEEDED(debug.As(&infoQueue)))
					{
						infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
						infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

						D3D11_MESSAGE_ID hide[] =
						{
							D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
						};

						D3D11_INFO_QUEUE_FILTER filter = {};
						filter.DenyList.NumIDs = _countof(hide);
						filter.DenyList.pIDList = hide;
						infoQueue->AddStorageFilterEntries(&filter);
					}
				}
			}
#endif // CGL_BUILD_DEBUG
		}

		CGL_LOG(D3D11RendererImpl, Trace, "D3D11 Device Created");
	}

	void D3D11RendererImpl::CreateSwapChain(u32 width, u32 height)
	{
		if (m_swapChain)
		{
			// Swapchain already exists, just resize it
			ResizeSwapchain(width, height);
			return;
		}

		assert(m_device && m_window);
		HRESULT hr{ S_OK };

		ComPtr<IDXGIDevice> dxgiDevice;
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ComPtr<IDXGIFactory> dxgiFactory;

		DXCall(hr = m_device.As(&dxgiDevice));
		DXCall(hr = dxgiDevice->GetParent(IID_IDXGIAdapter, (void**)&dxgiAdapter));
		DXCall(hr = dxgiAdapter->GetParent(IID_IDXGIFactory, (void**)&dxgiFactory));

		SetDebugObjectName(dxgiDevice.Get(), "DXGIDevice");
		SetDebugObjectName(dxgiAdapter.Get(), "DXGIAdapter");
		SetDebugObjectName(dxgiFactory.Get(), "DXGIFactory");

#ifdef CGL_BUILD_DEBUG
		{
			ComPtr<IDXGIDebug> dxgiDebug;
			if (SUCCEEDED(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
			{
				ComPtr<IDXGIInfoQueue> infoQueue;
				if (SUCCEEDED(dxgiDebug.As(&infoQueue)))
				{
					infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
					infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
				}
			}
		}
#endif // CGL_BUILD_DEBUG

		DXGI_RATIONAL refreshRateRational = {};
		IDXGIOutput* dxgiOutput = nullptr;
		u32 numModes = 0;

		// Use primary monitor
		DXCall(hr = dxgiAdapter->EnumOutputs(0, &dxgiOutput));
		DXCall(hr = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr));

		// Get refresh rate
		if (numModes > 0)
		{
			DXGI_MODE_DESC* modes = new DXGI_MODE_DESC[numModes];

			DXCall(hr = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, modes));

			u32 bestMatchIndex = u32(- 1);
			u32 bestError = u32max;
			for (u32 i = 0; i < numModes; ++i)
			{
				if (width != modes[i].Width || height != modes[i].Height)
					continue;

				f32 rate = (f32)modes[i].RefreshRate.Numerator / modes[i].RefreshRate.Denominator;
				u32 error = (u32)(abs(rate - 60));  // 60 is default refresh rate

				if (error < bestError)
				{
					bestMatchIndex = i;
					bestError = error;
				}
			}

			if (bestMatchIndex != -1)
			{
				refreshRateRational.Numerator = modes[bestMatchIndex].RefreshRate.Denominator;
				refreshRateRational.Denominator = modes[bestMatchIndex].RefreshRate.Numerator;
			}

			delete[] modes;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount                        = 1;
		swapChainDesc.BufferDesc.Width                   = width;
		swapChainDesc.BufferDesc.Height                  = height;
		swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.RefreshRate.Numerator   = refreshRateRational.Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateRational.Denominator;
		swapChainDesc.OutputWindow                       = m_window;
		swapChainDesc.SampleDesc.Count                   = 1;
		swapChainDesc.SampleDesc.Quality                 = 0;
		swapChainDesc.Windowed                           = TRUE;
		swapChainDesc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXCall(hr = dxgiFactory->CreateSwapChain(m_device.Get(), &swapChainDesc, &m_swapChain));
		assert(m_swapChain);

		SetDebugObjectName(m_swapChain.Get(), "D3D11Swapchain");

		dxgiFactory.Reset();
		dxgiAdapter.Reset();
		dxgiDevice.Reset();

		CGL_LOG(D3D11RendererImpl, Trace, "D3D11 SwapChain Created");
	}

	void D3D11RendererImpl::ResizeSwapchain(u32 width, u32 height)
	{
		// Ensure to call clear state + flush on the context before calling this
		assert(m_swapChain);

		HRESULT hr = m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// Shutdown and restart the DX11 renderer
			std::string err = (hr == DXGI_ERROR_DEVICE_REMOVED) ? "The device was removed" : "Error during device reset";
			CGL_LOG(D3D11RendererImpl, Error, "Swapchain resize error: {0}. Restarting...", err);

			Shutdown();
			Init();
		}

		CGL_LOG(D3D11RendererImpl, Trace, "D3D11 SwapChain Resized to {0}x{1}", width, height);
	}
}
