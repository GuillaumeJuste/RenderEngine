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
		Mesh* newMesh = new Mesh();
		renderContext->CreateMesh(rawMesh, newMesh);
		meshManager.Add(_filePath, newMesh);
		newMesh->filePath = _filePath;
		newMesh->indiceCount = rawMesh.indices.size();

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
		textureManager.Add(_filePath, newTexture);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;

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

void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
}