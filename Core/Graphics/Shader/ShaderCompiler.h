#pragma once
#include <Core/Graphics/Shader/ShaderCompileResult.h>
#include <Core/Graphics/Types.h>
#include <Core/Logging/Log.h>

#if defined(CGL_RHI_D3D)
#include <Core/Graphics/RHI/D3DCommon.h>
#include <d3dcompiler.h>
#endif

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(ShaderCompiler);

	struct ShaderSource
	{
		ShaderType Type;
		std::string FilePath;
	};

	struct CompileConfig
	{
		static constexpr u32 MAX_DEFINES = 16;

		std::string EntryPoint;
#if defined(CGL_RHI_D3D)
		std::string Target;
		std::array<D3D_SHADER_MACRO, MAX_DEFINES> Defines;
#endif
		bool Optimize = true;
		bool Debug = false;
		bool SkipValidate = false;
	};

	class ShaderCompiler
	{
	private:
#if defined(CGL_RHI_D3D)
		static ShaderCompileResult Compile(const ShaderSource& shader, const CompileConfig& config, ComPtr<ID3DBlob>& outBlob);
#endif
	};
}
