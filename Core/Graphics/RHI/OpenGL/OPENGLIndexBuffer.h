#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
	struct OPENGLIndexBuffer
	{
		u32 Stride;
		GLuint EBO;
	};
} // namespace CGL::Graphics