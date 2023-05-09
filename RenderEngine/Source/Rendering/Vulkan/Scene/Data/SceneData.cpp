#include "Rendering/Vulkan/Scene/Data/SceneData.hpp"

using namespace RenderEngine::Rendering;

bool SceneData::operator==(RenderEngine::SceneGraph::Scene* _scene)
{
	return scene == _scene;
}