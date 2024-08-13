#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	struct D3D11VertexShader
	{
		ComPtr<ID3DBlob>           Blob;
		ComPtr<ID3D11VertexShader> Shader;
		ComPtr<ID3D11InputLayout>  InputLayout;
	};
}
