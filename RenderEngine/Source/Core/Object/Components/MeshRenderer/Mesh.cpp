#include "Core/Object/Components/MeshRenderer/Mesh.hpp"

using namespace RenderEngine::Core;

void Mesh::InitializeMesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices, Mesh* _output)
{
	_output->indices = _indices;
	_output->vertices = _vertices;
}