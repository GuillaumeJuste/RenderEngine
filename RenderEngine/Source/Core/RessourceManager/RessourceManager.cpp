#include "Core/RessourceManager/RessourceManager.hpp"

#include <assimp/postprocess.h>

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


using namespace RenderEngine::Core;


RessourceManager* RessourceManager::GetInstance()
{
	static RessourceManager instance;
	return &instance;
}

Mesh* RessourceManager::LoadMesh(std::string _filePath)
{
	Mesh* mesh = GetMesh(_filePath);
	if (mesh != nullptr)
		return mesh;

	const aiScene* scene = importer.ReadFile(_filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene == nullptr) {
		throw std::runtime_error(importer.GetErrorString());
		return false;
	}

	Mesh* newMesh = &meshes.emplace_front();
	ProcessMesh(scene, newMesh);
	newMesh->filePath = _filePath;

	return newMesh;
}

void RessourceManager::ProcessMesh(const aiScene* _scene, Mesh* _output)
{
	if (_scene->HasMeshes())
	{
		aiMesh* mesh = _scene->mMeshes[0];

		_output->name = mesh->mName.C_Str();

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex newVertex;
			newVertex.position.X = mesh->mVertices[i].x;
			newVertex.position.Y = mesh->mVertices[i].y;
			newVertex.position.Z = mesh->mVertices[i].z;

			if (mesh->HasNormals())
			{
				newVertex.normal.X = mesh->mNormals[i].x;
				newVertex.normal.Y = mesh->mNormals[i].y;
				newVertex.normal.Z = mesh->mNormals[i].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				newVertex.textCoord.X = mesh->mTextureCoords[0][i].x;
				newVertex.textCoord.Y = mesh->mTextureCoords[0][i].y;
				newVertex.textCoord.Z = mesh->mTextureCoords[0][i].z;
			}

			_output->vertices.push_back(newVertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				_output->indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}

Mesh* RessourceManager::GetMesh(std::string _filepath)
{
	for (std::forward_list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (it->filePath == _filepath)
			return &(*it);
	}
	return nullptr;
}

bool RessourceManager::DeleteMesh(Mesh* _mesh)
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


Texture* RessourceManager::LoadTexture(std::string _filePath)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	int texWidth, texHeight, texChannels;

	char* data = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha));

	if (!data)
	{
		return nullptr;
	}

	Texture* newTexture = &textures.emplace_front();

	newTexture->width = texWidth;
	newTexture->height = texHeight;

	newTexture->imageSize = newTexture->width * newTexture->height * 4;
	newTexture->filePath = _filePath;

	newTexture->pixels.assign(data, data + newTexture->imageSize);

	stbi_image_free(data);

	return newTexture;
}

Texture* RessourceManager::GetTexture(std::string _filePath)
{
	for (std::forward_list<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->filePath == _filePath)
			return &(*it);
	}
	return nullptr;
}

bool RessourceManager::DeleteTexture(Texture* _texture)
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