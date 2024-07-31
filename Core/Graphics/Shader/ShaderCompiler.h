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
		std::string SourceData;
		std::string Name;
	};

	struct CompileConfig
	{
		std::string_view EntryPoint;
#if defined(CGL_RHI_D3D)
		std::string_view Target;
		std::vector<D3D_SHADER_MACRO> Defines;
#endif
		bool Optimize     = true;
		bool Debug        = false;
		bool SkipValidate = false;
	};

	class ShaderCompiler
	{
	public:
		static void ReportResult(const ShaderCompileResult& result, const std::string& source);

#if defined(CGL_RHI_D3D)
		static ShaderCompileResult Compile(const ShaderSource& shader, const CompileConfig& config, ComPtr<ID3DBlob>& outBlob);
#endif
	};
}
