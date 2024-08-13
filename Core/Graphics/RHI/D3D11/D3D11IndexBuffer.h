#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	struct D3D11IndexBuffer
	{
		u32 IndicesCount;
		DXGI_FORMAT Format;
		ComPtr<ID3D11Buffer> Buffer;
	};

}
