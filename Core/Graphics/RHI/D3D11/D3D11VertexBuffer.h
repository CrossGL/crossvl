#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
	class D3D11VertexBuffer
	{
		friend class Renderer;

	public:
		explicit D3D11VertexBuffer(ID3D11Buffer* buffer, u32 stride, u32 offset)
			: m_buffer(nullptr)
			, m_stride(stride)
			, m_offset(offset)
		{
			m_buffer.Attach(buffer);
		}

		~D3D11VertexBuffer() = default;
		
	private:
		ComPtr<ID3D11Buffer> m_buffer;
		u32 m_stride;
		u32 m_offset;
	};
}
