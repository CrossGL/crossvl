#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
	class OPENGLPixelShader
	{
		friend class Renderer;

	public:
		OPENGLPixelShader()
		{}

	private:
		GLuint  fragmentShader;
	};
} // namespace CGL::Graphics
