#include "Core/Object/Components/MeshRenderer/Mesh.hpp"

using namespace RenderEngine::Core;

void Mesh::InitializeMesh(std::vector<Vertex> _vertices, std::vector<uint16_t> _indices, Mesh* _output)
{
	_output->indices = _indices;
	_output->vertices = _vertices;
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
	return vertices;
}

const std::vector<uint16_t>& Mesh::GetIndices() const
{
	return indices;
}