#pragma once

#ifndef RENDERENGINE_IMAGEVIEWCREATEINFO
#define RENDERENGINE_IMAGEVIEWCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for ImageView Initialization
	*/
	struct ImageViewCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// swapchain images
		std::vector<VkImage> swapChainImages;

		/// swapchain images format 
		VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;

		/// default constructor
		ImageViewCreateInfo() = default;
	};
}

#endif