#ifndef RENDERENGINE_SHADERINCLUDER
#define RENDERENGINE_SHADERINCLUDER

#include <string>
#include <unordered_set>

#include <Windows.h>
#include "dxc/dxcapi.h"
#include <wrl.h> 

namespace Wrapper
{
	template <typename T>
	using CComPtr = Microsoft::WRL::ComPtr<T>;

	class ShaderIncluder : public IDxcIncludeHandler
	{
		CComPtr<IDxcUtils> mUtils;
	public:
		std::unordered_set<std::wstring> includedFiles;

		ShaderIncluder(CComPtr<IDxcUtils> _utils);

		HRESULT STDMETHODCALLTYPE LoadSource(_In_z_ LPCWSTR _pFilename,
			_COM_Outptr_result_maybenull_ IDxcBlob** _ppIncludeSource)override final;

		HRESULT QueryInterface(REFIID riid, void** ppvObject) override final;

		ULONG AddRef() override final;
		ULONG Release() override final;
	};
}

#endif