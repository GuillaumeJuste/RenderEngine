#include "Rendering/Vulkan/DeviceContext/QueueFamilyIndices.hpp"

using namespace RenderEngine::Rendering;

bool QueueFamilyIndices::isComplete()
{
	return graphicsFamily.has_value();
}