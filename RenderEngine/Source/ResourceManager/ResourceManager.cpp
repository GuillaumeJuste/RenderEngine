#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/Wrapper/AssimpWrapper.hpp"
#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <assimp/postprocess.h>

#include <stdexcept>
#include <cstring>
#include <filesystem>

using namespace RenderEngine;
using namespace RenderEngine::Wrapper;


ResourceManager::ResourceManager(IRenderContext* _renderContext) :
	renderContext { _renderContext }
{
}

Mesh* ResourceManager::LoadMesh(std::string _filePath)
{
	Mesh* mesh = GetMesh(_filePath);
	if (mesh != nullptr)
		return mesh;

	RawMesh rawMesh;
	if (AssimpWrapper::LoadMesh(_filePath, rawMesh))
	{
		Mesh tmp;
		renderContext->CreateMesh(rawMesh, tmp);
		Mesh* newMesh = meshManager.Add(_filePath, tmp);
		newMesh->filePath = _filePath;
		newMesh->indiceCount = rawMesh.indices.size();
		tmp.vertexBuffer = nullptr;
		tmp.indexBuffer = nullptr;

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

	Texture tmp;
	if (StbiWrapper::LoadTexture(_filePath, tmp))
	{
		Texture* newTexture = textureManager.Add(_filePath, tmp);
		newTexture->filePath = _filePath;

		return newTexture;
	}

	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string _filePath)
{
	return textureManager.Get(_filePath);
}

bool ResourceManager::DeleteTexture(Texture* _texture)
{
	return textureManager.Unload(_texture->filePath);
}

void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
}