#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/Wrapper/AssimpWrapper.hpp"
#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <assimp/postprocess.h>

#include <stdexcept>
#include <cstring>
#include <filesystem>

using namespace RenderEngine;
using namespace RenderEngine::Wrapper;


ResourceManager* ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return &instance;
}

Mesh* ResourceManager::LoadMesh(std::string _filePath)
{
	Mesh* mesh = GetMesh(_filePath);
	if (mesh != nullptr)
		return mesh;

	Mesh tmp;
	if (AssimpWrapper::LoadMesh(_filePath, tmp))
	{
		Mesh* newMesh = meshes.AddAsset(_filePath, tmp);
		newMesh->filePath = _filePath;

		return newMesh;
	}

	return nullptr;
}

Mesh* ResourceManager::GetMesh(std::string _filePath)
{
	return meshes.GetAsset(_filePath);
}

bool ResourceManager::DeleteMesh(Mesh* _mesh)
{
	return meshes.RemoveAsset(_mesh->filePath);
}


Texture* ResourceManager::LoadTexture(std::string _filePath)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	Texture tmp;
	if (StbiWrapper::LoadTexture(_filePath, tmp))
	{
		Texture* newTexture = textures.AddAsset(_filePath, tmp);
		newTexture->filePath = _filePath;

		return newTexture;
	}

	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string _filePath)
{
	return textures.GetAsset(_filePath);
}

bool ResourceManager::DeleteTexture(Texture* _texture)
{
	return textures.RemoveAsset(_texture->filePath);
}