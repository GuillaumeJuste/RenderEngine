#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/Wrapper/AssimpWrapper.hpp"
#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <stdexcept>
#include <cstring>
#include <fstream>

using namespace RenderEngine;
using namespace RenderEngine::Wrapper;


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

Cubemap* ResourceManager::LoadCubemap(CubemapImportInfos _filePaths, bool _computeMipmap)
{
	Cubemap* cubemap = GetCubemap(_filePaths);
	if (cubemap != nullptr)
		return cubemap;

	RawTexture rawCubemap;
	if (StbiWrapper::LoadCubemap(_filePaths, _computeMipmap, rawCubemap))
	{
		Cubemap* newCubemap = new Cubemap();
		renderContext->CreateCubemap(rawCubemap, newCubemap);
		newCubemap->filePath = _filePaths.pathes[0];
		newCubemap->height = rawCubemap.height;
		newCubemap->width = rawCubemap.width;
		newCubemap->mipLevels = rawCubemap.mipLevels;
		newCubemap->imageSize = rawCubemap.imageSize;
		cubemapManager.Add(_filePaths.pathes[0], newCubemap);

		StbiWrapper::FreeImage(rawCubemap.dataC);

		return newCubemap;
	}

	return nullptr;
}

Cubemap* ResourceManager::LoadCubemap(std::string _filePath, bool _isHDR, bool _computeMipmap)
{
	Cubemap* newCubemap = new Cubemap();

	Texture* skyboxTexture = LoadTexture(_filePath, _isHDR, _computeMipmap);
	Mesh* skyboxMesh = LoadMesh("Resources/Engine/Models/cube.obj");
	Shader* skyboxVertShader = LoadShader("Resources/Engine/Shaders/Skybox.vert.spv", VERTEX);
	Shader* skyboxFragShader = LoadShader("Resources/Engine/Shaders/TextureToCubemap.frag.spv", FRAGMENT);

	renderContext->CreateCubemap(skyboxTexture, skyboxMesh, skyboxVertShader, skyboxFragShader, newCubemap);
	newCubemap->filePath = _filePath;
	newCubemap->height = 1024;
	newCubemap->width = 1024;
	newCubemap->mipLevels = 1;
	newCubemap->imageSize = 1024 * 1024 * 4;
	if(_isHDR)
		newCubemap->imageSize *= 4;

	cubemapManager.Add(_filePath, newCubemap);

	return newCubemap;
}

Cubemap* ResourceManager::GetCubemap(const CubemapImportInfos& _filePaths)
{
	return cubemapManager.Get(_filePaths.pathes[0]);
}

bool ResourceManager::UnloadCubemap(Cubemap* _cubemap)
{
	return cubemapManager.Unload(_cubemap->filePath);
}

void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
	shaderManager.Clean();
	cubemapManager.Clean();
}