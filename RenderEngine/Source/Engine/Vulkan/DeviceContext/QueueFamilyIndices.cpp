#include "Engine/Vulkan/DeviceContext/QueueFamilyIndices.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool QueueFamilyIndices::isComplete()
{
	return graphicsFamily.has_value();
}