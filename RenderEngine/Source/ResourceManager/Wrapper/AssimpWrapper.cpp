#include "ResourceManager/Wrapper/AssimpWrapper.hpp"

#include <assimp/postprocess.h>
#include <stdexcept>
#include <cstring>

using namespace RenderEngine::Wrapper;

Assimp::Importer AssimpWrapper::importer = Assimp::Importer();

bool AssimpWrapper::ProcessMesh(const aiScene* _scene, Mesh& _output)
{
	if (_scene->HasMeshes())
	{
		aiMesh* mesh = _scene->mMeshes[0];

		_output.name = mesh->mName.C_Str();

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

			_output.vertices.push_back(newVertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				_output.indices.push_back(mesh->mFaces[i].mIndices[j]);
		}

		return true;
	}

	return false;
}

bool AssimpWrapper::LoadMesh(const std::string& _filePath, Mesh& _output)
{
	const aiScene* scene = importer.ReadFile(_filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene == nullptr) {
		throw std::runtime_error(importer.GetErrorString());
		return false;
	}

	if (ProcessMesh(scene, _output))
	{
		_output.filePath = _filePath;

		return true;
	}
	return false;
}