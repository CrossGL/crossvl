#pragma once

namespace CGL::Graphics
{
    template <typename T>
    void Renderer::CreateContantBuffer(const BufferSource& source, ConstantBuffer<T>& outBuffer)
    {
#if defined(CGL_RHI_DX11)
		    CreateContantBuffer_D3D11(source, outBuffer.Buffer);
#elif defined(CGL_RHI_OPENGL)
		    CreateConstantBuffer_OPENGL(source, outBuffer.Buffer);
#endif
    }

    template <typename T>
    void Renderer::SetConstantBufferData(const ConstantBuffer<T>& buffer, const T& data)
    {
#if defined(CGL_RHI_DX11)
		    SetConstantBufferData_D3D11(buffer.Buffer.Get(), static_cast<const void*>(&data), sizeof(T));
#elif defined(CGL_RHI_OPENGL)
		    SetConstantBufferData_OPENGL(buffer.Buffer.Get(), static_cast<const void*>(&data), sizeof(T));
#endif  
	}

	template <typename T> 
	void Renderer::SetContantBuffer(ShaderType shaderType, u32 startSlot, const ConstantBuffer<T>& buffer)
	{
#if defined(CGL_RHI_DX11)
		    SetContantBuffer_D3D11(shaderType, startSlot, buffer.Buffer);
#elif defined(CGL_RHI_OPENGL)
		    SetConstantBuffer_OPENGL(shaderType, startSlot, buffer.Buffer);
#endif 
	}
}