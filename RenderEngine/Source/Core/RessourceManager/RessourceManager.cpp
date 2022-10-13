#include "Core/RessourceManager/RessourceManager.hpp"

#include <assimp/postprocess.h>

#include <stdexcept>

using namespace RenderEngine::Core;


Mesh* RessourceManager::LoadMesh(std::string _name, std::string _filePath)
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

	MeshData* newMeshData = &meshes.emplace_front();
	ProcessMesh(scene, newMeshData);
	newMeshData->filePath = _filePath;
	newMeshData->mesh.name = _name;

	return &newMeshData->mesh;
}

void RessourceManager::ProcessMesh(const aiScene* _scene, MeshData* _output)
{
	if (_scene->HasMeshes())
	{
		aiMesh* mesh = _scene->mMeshes[0];
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(Vertex{ {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}, {1.f, 1.f, 1.f} });
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}

		Mesh::InitializeMesh(vertices, indices, &_output->mesh);
	}
}

Mesh* RessourceManager::GetMesh(std::string _name)
{
	for (std::forward_list<MeshData>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (it->mesh.name == _name)
			return &it->mesh;
	}
	return nullptr;
}

bool RessourceManager::DeleteMesh(Mesh* _mesh)
{
	for (std::forward_list<MeshData>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if (&it->mesh == _mesh)
		{
			meshes.remove(*it);
			return true;
		}
	}
	return false;
}