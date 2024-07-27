#include "ShaderCompiler.h"

#ifdef CGL_PLATFORM_WINDOWS
#include <Core/Platform/Win32/StringUtils.h>
#endif

namespace CGL::Graphics
{
	CGL_DEFINE_LOG_CATEGORY(ShaderCompiler);

#if defined(CGL_RHI_DX11) || defined(CGL_RHI_DX12)
	ShaderCompileResult ShaderCompiler::Compile(const ShaderSource& shader, const CompileConfig& config, ComPtr<ID3DBlob>& outBlob)
	{
		ShaderCompileResult result{};

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

		HRESULT hr{ S_OK };

		hr = D3DCompileFromFile(
			::CGL::Core::Platform::Win32::ToWideStr(shader.FilePath).c_str(),
			config.Defines.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			config.EntryPoint.c_str(),
			config.Target.c_str(),
			compileFlags,
			0,
			&shaderBlob,
			&errorBlob
		);

		// Failed to compile
		if (FAILED(hr) || shaderBlob == nullptr)
		{
			// Contains error message
			if (errorBlob)
			{
				const char* errorMsg = reinterpret_cast<const char*> (errorBlob->GetBufferPointer());
				result.Message = errorMsg;
				errorBlob->Release();
			}

			result.Status = ShaderCompileStatus::Failure;

			return result;
		}
		else
		{
			// Shader compilation succeeded

			// Contains warning message
			if (errorBlob)
			{
				const char* errorMsg = reinterpret_cast<const char*> (errorBlob->GetBufferPointer());
				result.Message = errorMsg;
				result.Status = ShaderCompileStatus::HasWarnings;
				errorBlob->Release();
			}
		}

		outBlob.Attach(shaderBlob); // Take ownership of shaderBlob;
		shaderBlob->Release();
	}
#endif
}
