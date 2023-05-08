#include "Engine/Vulkan/Scene/Data/SceneData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool SceneData::operator==(RenderEngine::SceneGraph::Scene* _scene)
{
	return scene == _scene;
}