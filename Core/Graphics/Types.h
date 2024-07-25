#pragma once

namespace CGL::Graphics
{
	enum class RHIType
	{
		None,
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan
	};

	enum class PrimitiveType
	{
		TriangleList,
		LineList,
		PointList,
		TriangleStrip,
		LineStrip,
		TriangleFan
	};

	enum class BlendMode
	{
		Replace,
		Add,
		Multiply,
		Alpha,
		AddAlpha,
		PreMulAlpha,
		InvDestAlpha,
		Subtract,
		SubtractAlpha,
		COUNT
	};

	enum class CompareMode
	{
		Always,
		Equal,
		NotEqual,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		COUNT
	};

	enum class CullMode
	{
		None,
		CCW,
		CW,
		COUNT
	};

	enum class FillMode
	{
		Solid,
		Wireframe,
		COUNT
	};

	enum class StencilOp
	{
		Keep,
		Zero,
		Ref,
		Increment,
		Decrement
	};

	enum class TextureFilterMode
	{
		Nearest,
		Bilinear,
		Trilinear,
		Anisotropic,
		NearestAnisotropic,
		Default,
		COUNT
	};

	enum class TextureAddressMode
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
		COUNT
	};

	enum class TextureCoordinates
	{
		U,
		V,
		W,
		COUNT
	};

	enum class TextureUsage
	{
		Static,
		Dynamic,
		RenderTarget,
		DepthStencil
	};

	enum class ShaderType
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		Library
	};
}
