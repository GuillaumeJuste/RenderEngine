#include "Engine/Vulkan/RenderContext/SceneData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool SceneData::operator==(RenderEngine::Core::Scene* _scene)
{
	return scene == _scene;
}