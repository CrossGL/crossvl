#include "ShaderCompiler.h"

#ifdef CGL_PLATFORM_WINDOWS
#include <Core/Platform/Win32/StringUtils.h>
#endif

namespace CGL::Graphics
{
	CGL_DEFINE_LOG_CATEGORY(ShaderCompiler);

	void ShaderCompiler::ReportResult(const ShaderCompileResult& result, const std::string& source)
	{
		if (result.Status == ShaderCompileStatus::Success)
		{
			CGL_LOG(ShaderCompiler, Info, "Successfully compiled shader: {}", source);
		}
		else if (result.Status == ShaderCompileStatus::Failure)
		{
			CGL_LOG(ShaderCompiler, Error, "Failed to compile shader: {}  Error: {}", source, result.Message);
		}
		else if (result.Status == ShaderCompileStatus::HasWarnings)
		{
			CGL_LOG(ShaderCompiler, Warn, "Shader compiled with warnings: {}  Warning: {}", source, result.Message);
		}
		else
		{
			CGL_LOG(ShaderCompiler, Error, "Failed to compile shader: {}  Error: {}", source, result.Message);
		}
	}

#if defined(CGL_RHI_D3D)
	ShaderCompileResult ShaderCompiler::Compile(const ShaderSource& shader, const CompileConfig& config, ComPtr<ID3DBlob>& outBlob)
	{
		ShaderCompileResult result{};

		if (shader.SourceData.empty())
		{
			result.Status = ShaderCompileStatus::Failure;
			result.Message = "Shader file path is empty";
			return result;
		}

		DWORD compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		if (config.Debug)
		{
			compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
		}
		else if (config.Optimize)
		{
			compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
		}
		else if (config.SkipValidate)
		{
			compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
		}

		ID3DBlob* errorBlob = nullptr;
		ID3DBlob* shaderBlob = nullptr;

		HRESULT hr = D3DCompile(
			shader.SourceData.c_str(),
			shader.SourceData.length(),
			shader.Name.c_str(),
			config.Defines.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			config.EntryPoint.data(),
			config.Target.data(),
			compileFlags,
			0,
			&shaderBlob,
			&errorBlob
		);

		if (FAILED(hr) || shaderBlob == nullptr)
		{
			if (errorBlob)  // If there was an error
			{
				const char* errorMsg = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
				result.Message = errorMsg;
				errorBlob->Release();
			}

			result.Status = ShaderCompileStatus::Failure;
			return result;
		}
		else
		{
			if (errorBlob)  // If there was a warning
			{
				const char* errorMsg = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
				result.Message = errorMsg;
				result.Status = ShaderCompileStatus::HasWarnings;
				errorBlob->Release();
			}

			result.Status = ShaderCompileStatus::Success;
			outBlob.Attach(shaderBlob);

			return result;
		}
	}

#endif
}
