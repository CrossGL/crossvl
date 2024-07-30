#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	class D3D11VertexShader
	{
		friend class Renderer;

	public:
		D3D11VertexShader()
			: m_blob(nullptr)
			, m_shader(nullptr)
			, m_layout(nullptr)
		{};

		
	private:
		std::string                m_path;
		ComPtr<ID3DBlob>           m_blob;
		ComPtr<ID3D11VertexShader> m_shader;
		ComPtr<ID3D11InputLayout>  m_layout;
	};
}
