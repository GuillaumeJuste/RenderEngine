#pragma once

#ifndef RENDERENGINE_COMMANDPOOLCREATEINFO
#define RENDERENGINE_COMMANDPOOLCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	struct CommandPoolCreateInfo
	{
		VkDevice* logicalDevice;
		uint32_t graphicsQueueIndex;

		CommandPoolCreateInfo() = default;
	};
}

#endif