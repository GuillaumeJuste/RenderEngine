#include "AssetLoader/AssetLoader.hpp"
using namespace Loader;

#include "AssetLoader/Wrapper/AssimpWrapper.hpp"
#include "AssetLoader/Wrapper/StbiWrapper.hpp"
using namespace Wrapper;

#include <fstream>

AssetLoader::AssetLoader()
{
	shaderCompiler.Create();
}

AssetLoader::~AssetLoader()
{
	shaderCompiler.Destroy();
}

bool AssetLoader::ReadShaderFile(const std::string& _shaderFilePath, RawShader& _output)
{
	std::ifstream file(_shaderFilePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		return false;
	}

	size_t fileSize = (size_t)file.tellg();
	_output.shaderCode = std::vector<char>(fileSize);

	file.seekg(0);
	file.read(_output.shaderCode.data(), fileSize);

	file.close();

	return true;
}

RawMesh AssetLoader::LoadMesh(std::string _filePath)
{
	RawMesh rawMesh;
	rawMesh.isValid = AssimpWrapper::LoadMesh(_filePath, rawMesh);
	return rawMesh;
}

RawTexture AssetLoader::LoadTexture(std::string _filePath, TextureFormat _format)
{
	RawTexture rawTexture;
	rawTexture.isValid = StbiWrapper::LoadTexture(_filePath, _format, rawTexture);
	return rawTexture;
}

RawTexture AssetLoader::LoadCubemap(const CubemapImportInfos& _filePaths, TextureFormat _format)
{
	RawTexture rawCubemap;
	rawCubemap.isValid = StbiWrapper::LoadCubemap(_filePaths, _format, rawCubemap);
	return rawCubemap;
}

RawShader AssetLoader::LoadShaderSPV(std::string _filePath)
{
	RawShader rawShader;
	rawShader.isValid = ReadShaderFile(_filePath, rawShader);
	return rawShader;
}

RawShader AssetLoader::LoadShaderHLSL(std::string _filePath, ShaderStage _shaderStage)
{
	ShaderCompileInfo compileInfo;
	compileInfo.path = _filePath;
	compileInfo.entrypoint = "main";

	switch (_shaderStage)
	{
	case Loader::VERTEX:
		compileInfo.target = "vs_6_5";
		break;
	case Loader::FRAGMENT:
		compileInfo.target = "ps_6_5";
		break;
	case Loader::COMPUTE:
		compileInfo.target = "cs_6_5";
		break;
	case Loader::GEOMETRY:
		compileInfo.target = "gs_6_5";
		break;
	default:
		break;
	}

	RawShader rawShader = shaderCompiler.CompileSPIRV(compileInfo);

	return rawShader;
}