#pragma once

#ifndef RENDERENGINE_COMMANDPOOLVKCREATEINFO
#define RENDERENGINE_COMMANDPOOLVKCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for CommandPool Initialization
	*/
	struct CommandPoolVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;
		
		/// graphics queue index
		uint32_t graphicsQueueIndex;

		/// default constructor
		CommandPoolVkCreateInfo() = default;
	};
}

#endif