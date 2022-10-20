#include "Engine/Vulkan/Scene/Data/MeshData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool MeshData::operator==(RenderEngine::Core::Mesh* _mesh) const
{
	return mesh == _mesh;
}

void MeshData::Cleanup()
{
	vertexBufferObject.Cleanup();
	indexBufferObject.Cleanup();
}