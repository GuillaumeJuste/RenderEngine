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
		Mesh* newMesh = &meshes.emplace_front(tmp);
		newMesh->filePath = _filePath;

		return newMesh;
	}

	return nullptr;
}

Mesh* ResourceManager::LoadMesh(std::vector<Vertex> _vertices, std::vector<uint16_t> _indices)
{
	Mesh* newMesh = &meshes.emplace_front();
	newMesh->vertices = _vertices;
	newMesh->indices = _indices;

	return newMesh;
}

Mesh* ResourceManager::GetMesh(std::string _filepath)
{
	for (std::forward_list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (it->filePath == _filepath)
			return &(*it);
	}
	return nullptr;
}

bool ResourceManager::DeleteMesh(Mesh* _mesh)
{
	for (std::forward_list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (&(*it) == _mesh)
		{
			meshes.remove(*it);
			return true;
		}
	}
	return false;
}


Texture* ResourceManager::LoadTexture(std::string _filePath)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	Texture tmp;
	if (StbiWrapper::LoadTexture(_filePath, tmp))
	{
		Texture* newTexture = &textures.emplace_front(tmp);
		newTexture->filePath = _filePath;

		return newTexture;
	}

	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string _filePath)
{
	for (std::forward_list<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->filePath == _filePath)
			return &(*it);
	}
	return nullptr;
}

bool ResourceManager::DeleteTexture(Texture* _texture)
{
	for (std::forward_list<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (&(*it) == _texture)
		{
			textures.remove(*it);
			return true;
		}
	}
	return false;
}