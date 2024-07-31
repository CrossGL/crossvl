#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	class D3D11IndexBuffer
	{
		friend class Renderer;

	public:
		D3D11IndexBuffer(ID3D11Buffer* buffer)
			: m_buffer(nullptr)
		{
			m_buffer.Attach(buffer);
		}

		~D3D11IndexBuffer() = default;

	private:
		ComPtr<ID3D11Buffer> m_buffer;
	};

}
