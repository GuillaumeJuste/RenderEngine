#pragma once

#ifndef RENDERENGINE_UTILS
#define RENDERENGINE_UTILS

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Rendering
{
	uint32_t FindMemoryType(VkPhysicalDevice _physicalDevice, uint32_t _typeFilter, VkMemoryPropertyFlags _properties);
}

#endif