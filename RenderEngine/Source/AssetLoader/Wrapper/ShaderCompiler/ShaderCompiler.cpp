#include "AssetLoader/Wrapper/ShaderCompiler/ShaderCompiler.hpp"
#include <iostream>

using namespace Wrapper;

void ShaderCompiler::Create()
{
	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&mUtils));
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&mCompiler));
}

void ShaderCompiler::Destroy()
{
	mCompiler = nullptr;
	mUtils = nullptr;
}

bool ShaderCompiler::ReadSourceShader(const std::string& _path, SourceBuffer& _src)
{
	if (FAILED(mUtils->LoadFile(ToWString(_path).c_str(), nullptr, &_src.blob)))
	{
		return false;
	}

	_src.dx.Ptr = _src.blob->GetBufferPointer();
	_src.dx.Size = _src.blob->GetBufferSize();
	_src.dx.Encoding = 0u;

	return true;
}

std::vector<LPCWSTR> ShaderCompiler::ProcessParams(const ShaderCompileInfo& _info, std::list<std::wstring>& _strBuff)
{
	std::wstring& wEntrypoint = _strBuff.emplace_back(ToWString(_info.entrypoint));
	std::wstring& wTarget = _strBuff.emplace_back(ToWString(_info.target));
	std::wstring& wStandard = _strBuff.emplace_back(ToWString(_info.standard));

	std::vector<LPCWSTR> cArgs
	{
		L"-E",
		wEntrypoint.c_str(),
		L"-T",
		wTarget.c_str(),
		L"-HV",
		wStandard.c_str(),
		DXC_ARG_WARNINGS_ARE_ERRORS,
		DXC_ARG_PACK_MATRIX_COLUMN_MAJOR,
		DXC_ARG_ALL_RESOURCES_BOUND,
		L"-I",
		L"/Resources/Engine/Shaders",
	};

	cArgs.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);


	cArgs.reserve(_info.defines.size() * 2);

	for (auto& define : _info.defines)
	{
		std::wstring& wDefine = _strBuff.emplace_back(ToWString(define));

		cArgs.push_back(L"-D");
		cArgs.push_back(wDefine.c_str());
	}

	return cArgs;
}

CComPtr<IDxcResult> ShaderCompiler::Compile_Internal(const DxcBuffer& _src,
	const std::vector<LPCWSTR>& _cArgs,
	const ShaderCompileInfo& _info)
{
	ShaderIncluder includer(mUtils);
	CComPtr<IDxcResult> compilResult;

	mCompiler->Compile(&_src,
		const_cast<LPCWSTR*>(_cArgs.data()),
		static_cast<uint32_t>(_cArgs.size()),
		&includer,
		IID_PPV_ARGS(&compilResult));

	CComPtr<IDxcBlobUtf8> errors;
	compilResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

	if (errors && errors->GetStringLength() > 0)
	{
		std::cout << _info.path.c_str() << " compilation failed" << errors->GetStringPointer() <<std::endl;
		return nullptr;
	}

	return compilResult;
}


Loader::RawShader ShaderCompiler::CompileSPIRV(const ShaderCompileInfo& _info)
{
	Loader::RawShader result;

	SourceBuffer src;
	if (!ReadSourceShader(_info.path, src))
		return result;


	std::list<std::wstring> strBuff;
	std::vector<LPCWSTR> cArgs = ProcessParams(_info, strBuff);
	cArgs.push_back(L"-spirv"); // Add SPIRV compile option.


	CComPtr<IDxcResult> compileResult = Compile_Internal(src.dx, cArgs, _info);

	if (!compileResult)
		return result;

	result.isValid = true;

	CComPtr<IDxcBlob> shader;
	compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader), nullptr);

	// Move to SPIRV-raw data type.
	result.shaderCode.resize(shader->GetBufferSize() / sizeof(decltype(result.shaderCode)::value_type));
	std::memmove(result.shaderCode.data(), shader->GetBufferPointer(), shader->GetBufferSize());

	std::cout << _info.path.c_str() << " compilation success" << std::endl;

	return result;
}

std::wstring ShaderCompiler::ToWString(const std::string& _str)
{
	if (_str.empty())
		return std::wstring();

	const int srcByteSize = static_cast<int>(_str.size() * sizeof(std::string::value_type));

	// Compute converted size.
	const int wsize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _str.data(), srcByteSize, nullptr, 0);

	std::wstring out;
	out.resize(wsize);

	// Convert.
	const int res = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _str.data(), srcByteSize, out.data(), wsize);

	(void)res;

	return out;
}