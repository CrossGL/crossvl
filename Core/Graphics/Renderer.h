#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Types.h>
#include <Core/Logging/Log.h>
#include <Core/Graphics/Buffer.h>
#include <Core/Graphics/Shader/Shader.h>
#include <Core/Graphics/Shader/ShaderCompiler.h>
#include <Core/Graphics/Material.h>

struct SDL_Window;

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(Renderer);

#if defined(CGL_RHI_DX11)
	class D3D11RendererImpl;
#elif defined(CGL_RHI_DX12)
	class D3D12RendererImpl;
#elif defined(CGL_RHI_OPENGL)
	class OPENGLRendererImpl;
#elif defined(CGL_RHI_METAL)
	class METALRendererImpl;
#elif defined(CGL_RHI_VULKAN)
	class VULKANRendererImpl;
#endif

	class Renderer
	{
	public:
		explicit Renderer(SDL_Window* window);
		~Renderer();

		inline u32 GetWidth() const noexcept { return m_width; }
		inline u32 GetHeight() const noexcept { return m_height; }

		void BeginFrame();
		void EndFrame();
		void Resize(u32 width, u32 height);

		void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
		void SetPrimitiveTopology(PrimitiveType topology);
		void SetVertexShader(const VertexShader& shader);
		void SetPixelShader(const PixelShader& shader);
		void SetMaterial(const Material& material);
		void SetVertexBuffer(const VertexBuffer& buffer);
		void SetIndexBuffer(const IndexBuffer& buffer);

		VertexBuffer CreateVertexBuffer(const BufferSource& source);
		IndexBuffer CreateIndexBuffer(const BufferSource& source);
		bool CompilePixelShader(const ShaderSource& source, PixelShader* outShader);
		bool CompileVertexShader(const ShaderSource& source, VertexShader* outShader);
		bool CompileMaterial(Material* material);

		template <typename T> void CreateContantBuffer(const BufferSource& source, ConstantBuffer<T>& outBuffer);
		template <typename T> void SetConstantBufferData(const ConstantBuffer<T>& buffer, const T& data);
		template <typename T> void SetContantBuffer(ShaderType shaderType, u32 startSlot, const ConstantBuffer<T>& buffer);

		void Draw(u32 vertexCount, u32 startVertex = 0);
		void DrawIndexed(u32 indexCount, u32 startIndex = 0, u32 baseVertex = 0);

	private:
#if defined(CGL_RHI_DX11)
		void Constructor_D3D11(SDL_Window* window);
		void Destructor_D3D11();
		void BeginFrame_D3D11();
		void EndFrame_D3D11();
		void Resize_D3D11(u32 width, u32 height);
		void SetPrimitiveTopology_D3D11(PrimitiveType topology);
		void SetVertexShader_D3D11(const VertexShader& shader);
		void SetPixelShader_D3D11(const PixelShader& shader);
		void SetVertexBuffer_D3D11(const VertexBuffer& buffer);
		void SetIndexBuffer_D3D11(const IndexBuffer& buffer);
		ShaderCompileResult CompileVertexShader_D3D11(const ShaderSource& source, VertexShader* outShader);
		ShaderCompileResult CompilePixelShader_D3D11(const ShaderSource& source, PixelShader* outShader);
		void CreateContantBuffer_D3D11(const BufferSource& source, ComPtr<ID3D11Buffer>& outBuffer);
		void SetConstantBufferData_D3D11(ID3D11Buffer* buffer, const void* data, size_t size);
		void SetContantBuffer_D3D11(ShaderType type, u32 startSlot, const ComPtr<ID3D11Buffer>& buffer);
		VertexBuffer CreateVertexBuffer_D3D11(const BufferSource& source);
		IndexBuffer CreateIndexBuffer_D3D11(const BufferSource& source);
		void Draw_D3D11(u32 vertexCount, u32 startVertex = 0);
		void DrawIndexed_D3D11(u32 indexCount, u32 startIndex = 0, u32 baseVertex = 0);
		D3D11RendererImpl* GetImpl() const;
#elif defined(CGL_RHI_OPENGL)
		void Constructor_OPENGL(SDL_Window* window);
		void Destructor_OPENGL();
		void BeginFrame_OPENGL();
		void EndFrame_OPENGL();
		void Resize_OPENGL(u32 width, u32 height);
		OPENGLRendererImpl* GetImpl() const;
#elif defined(CGL_RHI_METAL)
		void Constructor_METAL(SDL_Window* window);
		void Destructor_METAL();
		void BeginFrame_METAL();
		void EndFrame_METAL();
		void Resize_METAL(u32 width, u32 height);
		METALRendererImpl* GetImpl() const;
#endif

	private:
		void* m_impl;
		std::array<f32, 4> m_clearColor;
		u32 m_width;
		u32 m_height;
	};
}

// Contains template bodies
#include "Renderer.inl"
