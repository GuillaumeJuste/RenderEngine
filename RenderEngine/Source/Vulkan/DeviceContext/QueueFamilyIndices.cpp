#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"

using namespace RenderEngine::Vulkan;

bool QueueFamilyIndices::isComplete()
{
	return graphicsFamily.has_value() && presentFamily.has_value();
}