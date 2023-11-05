#ifndef RENDERENGINE_SHADERCOMPILER
#define RENDERENGINE_SHADERCOMPILER

#include "ShaderIncluder.hpp"
#include "ShaderCompileInfo.hpp"
#include "AssetLoader/RawAsset/Shader/RawShader.hpp"

namespace Wrapper
{
	class ShaderCompiler
	{
		CComPtr<IDxcUtils> mUtils;
		CComPtr<IDxcCompiler3> mCompiler;

		struct SourceBuffer
		{
			CComPtr<IDxcBlobEncoding> blob;
			DxcBuffer dx;
		};

		bool ReadSourceShader(const std::string& _path, SourceBuffer& _src);
		std::vector<LPCWSTR> ProcessParams(const ShaderCompileInfo& _info, std::list<std::wstring>& _strBuff);
		CComPtr<IDxcResult> Compile_Internal(const DxcBuffer& _src,
			const std::vector<LPCWSTR>& _cArgs,
			const ShaderCompileInfo& _info);

		std::wstring ToWString(const std::string& _str);

	public:
		void Create();
		void Destroy();

		Loader::RawShader CompileSPIRV(const ShaderCompileInfo& _info);
	};
}

#endif