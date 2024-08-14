#pragma once
#include <Core/Math/Math.h>

namespace CGL::Graphics
{
	enum class RHIType
	{
		None,
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan,
		Metal
	};

	enum class PrimitiveType
	{
		TriangleList,
		LineList,
		PointList,
		TriangleStrip,
		LineStrip,
		COUNT
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

	enum class BufferUsage
	{
		Default,
		Immutable,
		Dynamic,
		Staging,
		COUNT
	};

	enum class BufferType
	{
		None,
		Vertex,
		Index,
		Constant
	};

	namespace VertexTypes
	{
		struct Position
		{
			SM::Vector3 Position;
		};

		struct PositionColor
		{
			SM::Vector3 Position;
			SM::Vector4 Color;
		};

		struct PositionTexture
		{
			SM::Vector3 Position;
			SM::Vector2 Texture;
		};

		struct PositionColorTexture
		{
			SM::Vector3 Position;
			SM::Vector4 Color;
			SM::Vector2 Texture;
		};
	}
}