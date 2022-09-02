#pragma once

#ifndef RENDERENGINE_COMMANDPOOLCREATEINFO
#define RENDERENGINE_COMMANDPOOLCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for CommandPool Initialization
	*/
	struct CommandPoolCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;
		
		/// graphics queue index
		uint32_t graphicsQueueIndex;

		/// default constructor
		CommandPoolCreateInfo() = default;
	};
}

#endif