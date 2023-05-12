#include "ResourceManager/Assets/Mesh/Mesh.hpp"

using namespace RenderEngine::Assets;

Mesh::~Mesh()
{
	Clean();
}

void Mesh::Clean()
{
	if(vertexBuffer != nullptr)
		vertexBuffer->Clean();
	
	if (indexBuffer != nullptr)
		indexBuffer->Clean();
}