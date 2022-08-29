#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERCREATEINFO
#define RENDERENGINE_COMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{
	struct CommandBufferCreateInfo
	{
		/// VkDevice to initialize VkBuffer
		VkDevice logicalDevice;

		///Command Pol owning the command buffer
		CommandPool* commandPool;


		CommandBufferCreateInfo() = default;
	};
}

#endif