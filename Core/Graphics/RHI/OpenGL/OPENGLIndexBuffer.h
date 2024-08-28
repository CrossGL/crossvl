#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
	struct OPENGLIndexBuffer
	{
		u32 IndicesCount;
		u32 Size;
		GLuint EBO;
	};
} // namespace CGL::Graphics