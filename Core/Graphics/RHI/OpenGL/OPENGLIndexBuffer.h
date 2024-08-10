#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
	class OPENGLIndexBuffer
	{
		friend class Renderer;

	public:
		OPENGLIndexBuffer()
		{}
		~OPENGLIndexBuffer() = default;

	private:
		GLuint EBO;
	};
} // namespace CGL::Graphics