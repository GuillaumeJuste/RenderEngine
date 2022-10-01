#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO
#define RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetLayoutVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		DescriptorSetLayoutVkCreateInfo() = default;
	};
}

#endif