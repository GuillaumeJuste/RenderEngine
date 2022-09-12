#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERVKCREATEINFO
#define RENDERENGINE_COMMANDBUFFERVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Engine::Vulkan
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