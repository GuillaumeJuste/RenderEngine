#include "Rendering/Vulkan/Scene/Data/MeshData.hpp"

using namespace RenderEngine::Rendering;

bool MeshData::operator==(RenderEngine::Assets::Mesh* _mesh) const
{
	return mesh == _mesh;
}

void MeshData::Cleanup()
{
	vertexBufferObject.Cleanup();
	indexBufferObject.Cleanup();
}