#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERCREATEINFO
#define RENDERENGINE_COMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for CommandBuffer Initialization
	*/
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