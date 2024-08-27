#pragma once

#include <GL/glew.h>
#include <Core/Graphics/Types.h>

namespace CGL::Graphics
{
	template <typename>
	void OPENGLEnableVertexAttributes();

	template <>
	void OPENGLEnableVertexAttributes<VertexTypes::Position>();

	template <>
	void OPENGLEnableVertexAttributes<VertexTypes::PositionColor>();

	template <>
	void OPENGLEnableVertexAttributes<VertexTypes::PositionTexture>();

	template <>
	void OPENGLEnableVertexAttributes<VertexTypes::PositionColorTexture>();
}