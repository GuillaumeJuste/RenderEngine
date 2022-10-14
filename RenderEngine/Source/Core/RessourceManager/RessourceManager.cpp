#include "Core/RessourceManager/RessourceManager.hpp"

#include <assimp/postprocess.h>

#include <stdexcept>

using namespace RenderEngine::Core;


Mesh* RessourceManager::LoadMesh(std::string _filePath, std::string _name)
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

		_output->name = mesh->mName.C_Str();
	}
}

Mesh* RessourceManager::GetMesh(std::string _name)
{
	for (std::forward_list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (it->name == _name)
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