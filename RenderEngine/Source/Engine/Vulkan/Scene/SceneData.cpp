#include "Engine/Vulkan/Scene/SceneData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool SceneData::operator==(RenderEngine::Core::Scene* _scene)
{
	return scene == _scene;
}