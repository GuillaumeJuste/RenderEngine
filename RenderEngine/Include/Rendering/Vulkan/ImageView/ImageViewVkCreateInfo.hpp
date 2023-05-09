#pragma once

#ifndef RENDERENGINE_IMAGEVIEWVKCREATEINFO
#define RENDERENGINE_IMAGEVIEWVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Rendering
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

		VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;

		uint32_t layerCount = 1;

		VkImageAspectFlags aspectFlags;

		ImageViewVkCreateInfo() = default;
	};
}

#endif