#include "Engine/Device/QueueFamiliyIndices.hpp"

using namespace RenderEngine;

bool QueueFamilyIndices::isComplete()
{
	return graphicsFamily.has_value() && presentFamily.has_value();
}