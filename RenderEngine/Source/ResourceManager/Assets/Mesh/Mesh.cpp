#include "ResourceManager/Assets/Mesh/Mesh.hpp"

using namespace RenderEngine::Assets;

Mesh::~Mesh()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Clean();
		delete vertexBuffer;
	}
	if (indexBuffer != nullptr)
	{
		indexBuffer->Clean();
		delete indexBuffer;
	}
}