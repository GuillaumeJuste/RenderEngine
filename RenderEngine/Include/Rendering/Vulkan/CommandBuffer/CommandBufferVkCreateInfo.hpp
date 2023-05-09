#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERVKCREATEINFO
#define RENDERENGINE_COMMANDBUFFERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Rendering
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