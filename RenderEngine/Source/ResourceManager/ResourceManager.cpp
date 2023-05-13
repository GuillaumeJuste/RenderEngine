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

Texture* ResourceManager::LoadTexture(std::string _filePath)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	RawTexture rawTexture;
	rawTexture.textureCount = 1;
	if (StbiWrapper::LoadTexture(_filePath, rawTexture))
	{
		Texture* newTexture = new Texture();
		renderContext->CreateTexture(rawTexture, newTexture);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;
		textureManager.Add(_filePath, newTexture);

		StbiWrapper::FreeImage(rawTexture.pixels);

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

void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
	shaderManager.Clean();
}