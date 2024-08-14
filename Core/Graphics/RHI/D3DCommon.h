#pragma once
#include <Core/Common.h>
#include <Core/Platform/Win32/Error/HResultError.h>
#include <Core/Logging/Log.h>
#include <dxgi1_6.h>
#include <d3d11shader.h>
#include <wrl/client.h>
#include <exception>

#if defined(CGL_RHI_DX11)
#include <d3d11.h>
#elif defined(CGL_RHI_DX12)
#include <d3d12.h>
#endif

#ifdef CGL_BUILD_DEBUG
#include <dxgidebug.h>
#endif // CGL_BUILD_DEBUG

namespace DirectX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) noexcept : result(hr) {}

		const char* what() const noexcept override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

		HRESULT get_result() const noexcept { return result; }

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr) noexcept(false)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}

namespace CGL::Graphics
{
#ifdef CGL_BUILD_DEBUG
#define DXCall(x)                                                                  \
{                                                                                  \
	try                                                                            \
	{                                                                              \
		DirectX::ThrowIfFailed(x);                                                 \
	}                                                                              \
	catch (const DirectX::com_exception& e)                                        \
	{                                                                              \
		CGL_DECLARE_LOG_CATEGORY_INLINE(D3DException);                             \
		std::string msg(e.what());                                                 \
		msg += " (" + ::CGL::Platform::Win32::GetErrorString(e.get_result()) + ")";\
		CGL_LOG(D3DException, Fatal, msg);                                         \
	}                                                                              \
}
#else
#define DXCall(x) x
#endif // CGL_BUILD_DEBUG

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	template <class T>
	void SafeRelease(T** ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}
}
