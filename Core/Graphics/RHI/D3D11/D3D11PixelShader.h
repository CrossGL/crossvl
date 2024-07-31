#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	class D3D11PixelShader
	{
		friend class Renderer;

	public:
		D3D11PixelShader()
			: m_blob(nullptr)
			, m_shader(nullptr)
		{};
		
	private:
		std::string                m_path;
		ComPtr<ID3DBlob>           m_blob;
		ComPtr<ID3D11PixelShader>  m_shader;
	};
}
