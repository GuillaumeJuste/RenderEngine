#include "Core/Object/Mesh/Mesh.hpp"

using namespace RenderEngine::Core;
using namespace RenderEngine::Vulkan;

void Mesh::InitializeMesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices, Mesh* _output)
{
	_output->indices = _indices;
	_output->vertices = _vertices;
}

const BufferObject* Mesh::GetVertexBufferObject() const
{
	return &vertexBufferObject;
}

const BufferObject* Mesh::GetIndexBufferObject() const
{
	return &indexBufferObject;
}