#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERVKCREATEINFO
#define RENDERENGINE_COMMANDBUFFERVKCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for CommandBuffer Initialization
	*/
	struct CommandBufferVkCreateInfo
	{
		/// VkDevice to initialize VkBuffer
		VkDevice logicalDevice;

		///Command Pol owning the command buffer
		CommandPool* commandPool;


		CommandBufferVkCreateInfo() = default;
	};
}

#endif