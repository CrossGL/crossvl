#pragma once
#include <GL/glew.h>

namespace CGL::Graphics
{
	template <typename T>
	struct OPENGLConstantBuffer
	{
		using value_type = T;

		OPENGLConstantBuffer()
		{
			// Ensure 16-byte alignment
			static_assert((sizeof(T) % 16) == 0, "Constant buffer size must be 16-byte aligned.");
		}
		GLuint Buffer;
	};
} // namespace CGL::Graphics