#pragma once

#ifndef RENDERENGINE_IMAGEVIEWVKCREATEINFO
#define RENDERENGINE_IMAGEVIEWVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for ImageView Initialization
	*/
	struct ImageViewVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// swapchain images
		VkImage image;

		/// swapchain images format 
		VkFormat format = VK_FORMAT_UNDEFINED;

		VkImageAspectFlags aspectFlags;

		ImageViewVkCreateInfo() = default;
	};
}

#endif