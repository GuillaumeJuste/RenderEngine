#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/Wrapper/AssimpWrapper.hpp"
#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <stdexcept>
#include <cstring>
#include <fstream>
#include <filesystem>

using namespace RenderEngine;
using namespace RenderEngine::Wrapper;

#define ASSETPATH "Resources/Engine/Assets/"

ResourceManager::ResourceManager(IRenderContext* _renderContext) :
	renderContext { _renderContext }
{
}

bool ResourceManager::ReadShaderFile(const std::string& _shaderFilePath, RawShader& _output)
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

Mesh* ResourceManager::LoadMesh(std::string _filePath)
{
	Mesh* mesh = GetMesh(_filePath);
	if (mesh != nullptr)
		return mesh;

	RawMesh rawMesh;
	if (AssimpWrapper::LoadMesh(_filePath, rawMesh))
	{
		Mesh* newMesh = new Mesh();
		renderContext->CreateMesh(rawMesh, newMesh);
		newMesh->filePath = _filePath;
		newMesh->indiceCount = rawMesh.indices.size();
		meshManager.Add(_filePath, newMesh);

		return newMesh;
	}

	return nullptr;
}

Mesh* ResourceManager::GetMesh(std::string _filePath)
{
	return meshManager.Get(_filePath);
}

bool ResourceManager::UnloadMesh(Mesh* _mesh)
{
	return meshManager.Unload(_mesh->filePath);
}

Texture* ResourceManager::LoadTexture(std::string _filePath, bool _isHDR, bool _computeMipmap)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	RawTexture rawTexture;
	if (StbiWrapper::LoadTexture(_filePath, _isHDR, _computeMipmap, rawTexture))
	{
		Texture* newTexture = new Texture();
		renderContext->CreateTexture(rawTexture, newTexture);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;
		newTexture->mipLevels = rawTexture.mipLevels;
		newTexture->imageCount = 1;
		newTexture->isHDR = _isHDR;
		textureManager.Add(_filePath, newTexture);

		StbiWrapper::FreeImage(rawTexture.dataC);

		return newTexture;
	}

	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string _filePath)
{
	return textureManager.Get(_filePath);
}

bool ResourceManager::UnloadTexture(Texture* _texture)
{
	return textureManager.Unload(_texture->filePath);
}

Shader* ResourceManager::LoadShader(std::string _filePath, SHADER_STAGE _shaderStage)
{
	Shader* shader = GetShader(_filePath);
	if (shader != nullptr)
		return shader;

	RawShader rawShader;
	if (ReadShaderFile(_filePath, rawShader))
	{
		Shader* newShader = new Shader();
		rawShader.stage = _shaderStage;
		renderContext->CreateShader(rawShader, newShader);
		newShader->filePath = _filePath;
		newShader->stage = _shaderStage;
		shaderManager.Add(_filePath, newShader);

		return newShader;
	}

	return nullptr;
}

Shader* ResourceManager::GetShader(std::string _filePath)
{
	return shaderManager.Get(_filePath);
}

bool ResourceManager::UnloadShader(Shader* _texture)
{
	return shaderManager.Unload(_texture->filePath);
}

Texture* ResourceManager::LoadCubemap(CubemapImportInfos _filePaths, std::string _assetName, bool _computeMipmap)
{
	std::filesystem::path filepath(_filePaths.right);
	filepath.remove_filename();
	filepath.append(_assetName);

	Texture* cubemap = GetCubemap(filepath.string());
	if (cubemap != nullptr)
		return cubemap;

	RawTexture rawCubemap;
	if (StbiWrapper::LoadCubemap(_filePaths, _computeMipmap, rawCubemap))
	{
		Texture* newCubemap = new Texture();
		renderContext->CreateTexture(rawCubemap, newCubemap);
		newCubemap->filePath = filepath.string();
		newCubemap->height = rawCubemap.height;
		newCubemap->width = rawCubemap.width;
		newCubemap->mipLevels = rawCubemap.mipLevels;
		newCubemap->imageSize = rawCubemap.imageSize;
		newCubemap->imageCount = 6;
		newCubemap->isHDR = false;
		textureManager.Add(newCubemap->filePath, newCubemap);

		StbiWrapper::FreeImage(rawCubemap.dataC);

		return newCubemap;
	}

	return nullptr;
}

Texture* ResourceManager::CubemapFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, bool _computeMipmap)
{
	std::filesystem::path filename(_texture->filePath);
	filename.replace_extension();
	filename.concat("Cubemap");

	Texture* cubemap = GetCubemap(filename.string());
	if (cubemap != nullptr)
		return cubemap;

	Texture* newCubemap = new Texture();

	Mesh* skyboxMesh = LoadMesh("Resources/Engine/Models/cube.obj");
	Shader* skyboxVertShader = LoadShader("Resources/Engine/Shaders/FilterCube.vert.spv", VERTEX);
	Shader* skyboxFragShader = LoadShader("Resources/Engine/Shaders/TextureToCubemap.frag.spv", FRAGMENT);

	renderContext->CreateCubemap(_texture->iTexture, _generatedTextureSize, _computeMipmap, newCubemap, skyboxMesh, skyboxVertShader, skyboxFragShader);
	newCubemap->filePath = filename.string();
	newCubemap->isHDR = true;

	textureManager.Add(newCubemap->filePath, newCubemap);

	return newCubemap;
}

Texture* ResourceManager::GetCubemap(std::string _filePath)
{
	return textureManager.Get(_filePath);
}

bool ResourceManager::UnloadCubemap(Texture* _cubemap)
{
	return textureManager.Unload(_cubemap->filePath);
}

void ResourceManager::CreateSkyboxFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, RenderEngine::SceneGraph::Skybox* _output)
{
	_output->cubemap = CubemapFromTexture(_texture, _generatedTextureSize, _output->cubemap);

	Mesh* skyboxMesh = LoadMesh("Resources/Engine/Models/cube.obj");
	Shader* skyboxVertShader = LoadShader("Resources/Engine/Shaders/FilterCube.vert.spv", VERTEX);

	std::filesystem::path irradianceFilename(_texture->filePath);
	irradianceFilename.replace_extension();
	irradianceFilename.concat("Irradiance");

	Texture* irradianceCubemap = GetCubemap(irradianceFilename.string());
	if (irradianceCubemap != nullptr)
		_output->irradianceMap = irradianceCubemap;
	else
	{
		irradianceCubemap = new Texture();

		Shader* irradianceFragShader = LoadShader("Resources/Engine/Shaders/IrradianceConvolution.frag.spv", FRAGMENT);

		renderContext->CreateCubemap(_output->cubemap->iTexture, _generatedTextureSize, false, irradianceCubemap, skyboxMesh, skyboxVertShader, irradianceFragShader);
		irradianceCubemap->filePath = irradianceFilename.string();
		irradianceCubemap->isHDR = true;

		textureManager.Add(irradianceCubemap->filePath, irradianceCubemap);

		_output->irradianceMap = irradianceCubemap;
	}

	std::filesystem::path prefilteredFilename(_texture->filePath);
	prefilteredFilename.replace_extension();
	prefilteredFilename.concat("Prefiltered");

	Texture* prefilterCubemap = GetCubemap(prefilteredFilename.string());
	if (prefilterCubemap != nullptr)
		_output->prefilterMap = prefilterCubemap;
	else
	{
		prefilterCubemap = new Texture();

		Shader* prefilterFragShader = LoadShader("Resources/Engine/Shaders/PrefilterEnvmap.frag.spv", FRAGMENT);

		renderContext->CreatePrefilteredCubemap(_output->cubemap->iTexture, _generatedTextureSize, prefilterCubemap, skyboxMesh, skyboxVertShader, prefilterFragShader);
		prefilterCubemap->filePath = prefilteredFilename.string();
		prefilterCubemap->isHDR = true;

		textureManager.Add(prefilterCubemap->filePath, prefilterCubemap);

		_output->prefilterMap = prefilterCubemap;
	}
}

std::vector<char> fileData;

void ResourceManager::SaveAsset(Texture* _texture)
{
	std::vector<char> data = renderContext->GetTextureContent(_texture);

	fileData = data;

	std::filesystem::path filePath(_texture->filePath);
	if (filePath.has_extension())
		filePath.replace_extension();
	filePath.concat(".asset");

	std::ofstream ofs;
	ofs.open(filePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	ofs << _texture->width << ";" << _texture->height << ";" << _texture->imageSize << ";" << _texture->imageCount << ";" << _texture->mipLevels << ";" << _texture->isHDR << ";";
	ofs.write(data.data(), data.size());
	
	ofs.close();
}

size_t stringCompare(std::vector<char> _first, std::string _second)
{
	int string1Size = _first.size();
	int string2Size = _second.length();

	size_t total = 0;
	for (size_t i = 0; i < Mathlib::Math::Min(string1Size, string2Size); i++)
	{
		if (_first[i] == _second[i])
		{
			total++;
		}
		/*else
			return total;*/
	}
	return total;
}

Texture* ResourceManager::LoadAsset(std::string _filePath)
{
	std::filesystem::path filename(_filePath);
	if (filename.extension() == ".asset")
	{
		std::ifstream file(_filePath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return { };

		// Read contents
		std::string content{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

		// Close the file
		file.close();
		
		std::vector<std::string> output;
		size_t oldIndex = 0;
		size_t currentIndex = 0;

		for (int i = 0; i <= 5; i++)
		{
			currentIndex = content.find(';', oldIndex);
			output.push_back(content.substr(oldIndex, currentIndex - oldIndex));
			oldIndex = currentIndex + 1;
		}
		output.push_back(content.substr(oldIndex));

		RawTexture rawTexture;
		rawTexture.width = std::stoi(output[0]);
		rawTexture.height = std::stoi(output[1].c_str());
		rawTexture.imageSize = std::stoi(output[2].c_str());
		rawTexture.imageCount = std::stoi(output[3].c_str());
		//rawTexture.mipLevels = std::stoi(output[4].c_str());
		rawTexture.mipLevels = 1;
		rawTexture.isHdr = std::stoi(output[5].c_str());


		if (rawTexture.isHdr)
			rawTexture.dataF = reinterpret_cast<float*>(output[6].data());
		else
			rawTexture.dataC = output[6].data();

		size_t fileSize = content.length();
		size_t dataSize = output[6].length();
		int totalsize = rawTexture.imageCount * rawTexture.imageSize;

		size_t common = stringCompare(fileData, output[6]);

		Texture* newTexture = new Texture();
		renderContext->CreateTexture(rawTexture, newTexture);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;
		newTexture->mipLevels = rawTexture.mipLevels;
		newTexture->imageCount = rawTexture.imageCount;
		newTexture->isHDR = rawTexture.isHdr;
		textureManager.Add(_filePath, newTexture);

		return newTexture;
	}

}


void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
	shaderManager.Clean();
}