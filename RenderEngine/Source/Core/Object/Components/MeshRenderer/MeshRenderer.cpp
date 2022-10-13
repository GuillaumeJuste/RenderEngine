#include "Core/Object/Components/MeshRenderer/MeshRenderer.hpp"

using namespace RenderEngine::Core;

void MeshRenderer::Initialize()
{
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Update()
{
}

Mesh* MeshRenderer::GetMesh()
{
	return mesh;
}

void MeshRenderer::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}
