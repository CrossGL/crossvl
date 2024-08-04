#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	template <typename T>
	struct D3D11ConstantBuffer
	{
		using value_type = T;

		D3D11ConstantBuffer()
		{
			// Ensure 16-byte alignment
			static_assert((sizeof(T) % 16) == 0, "Constant buffer size must be 16-byte aligned.");
		}

		ComPtr<ID3D11Buffer> Buffer;
	};
}