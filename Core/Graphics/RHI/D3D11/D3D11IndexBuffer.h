#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	struct D3D11IndexBuffer
	{
		ComPtr<ID3D11Buffer> Buffer;
	};

}
