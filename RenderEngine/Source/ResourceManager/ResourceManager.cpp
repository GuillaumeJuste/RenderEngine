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
	ofs.write(reinterpret_cast<const char*>(&_texture->width), sizeof(int));
	ofs.write(reinterpret_cast<const char*>(&_texture->height), sizeof(int));
	ofs.write(reinterpret_cast<const char*>(&_texture->imageSize), sizeof(int));
	ofs.write(reinterpret_cast<const char*>(&_texture->imageCount), sizeof(uint32_t));
	ofs.write(reinterpret_cast<const char*>(&_texture->mipLevels), sizeof(uint32_t));
	ofs.write(reinterpret_cast<const char*>(&_texture->isHDR), sizeof(bool));
	ofs.write(data.data(), data.size());
	
	ofs.close();
}

Texture* ResourceManager::LoadAsset(std::string _filePath)
{
	std::filesystem::path filename(_filePath);
	if (filename.extension() == ".asset")
	{
		std::ifstream file(_filePath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return { };

		RawTexture rawTexture;

		file.read(reinterpret_cast<char*>(&rawTexture.width), sizeof(int));
		file.read(reinterpret_cast<char*>(&rawTexture.height), sizeof(int));
		file.read(reinterpret_cast<char*>(&rawTexture.imageSize), sizeof(int));
		file.read(reinterpret_cast<char*>(&rawTexture.imageCount), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&rawTexture.mipLevels), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&rawTexture.isHdr), sizeof(bool));

		if (rawTexture.isHdr)
		{
			rawTexture.dataF = new float[rawTexture.imageCount * rawTexture.imageSize];
			file.read(reinterpret_cast<char*>(rawTexture.dataF), rawTexture.imageCount * rawTexture.imageSize);
		}
		else
		{
			rawTexture.dataC = new char[rawTexture.imageCount * rawTexture.imageSize];
			file.read(rawTexture.dataC, rawTexture.imageCount * rawTexture.imageSize);
		}

		file.close();


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

		if (rawTexture.isHdr)
		{
			delete rawTexture.dataF;
		}
		else
		{
			delete rawTexture.dataC;
		}

		return newTexture;
	}

}


void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
	shaderManager.Clean();
}