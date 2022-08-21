#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERCREATEINFO
#define RENDERENGINE_COMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{
	struct CommandBufferCreateInfo
	{
		VkDevice logicalDevice;
		CommandPool* commandPool;


		CommandBufferCreateInfo() = default;
	};
}

#endif