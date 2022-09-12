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
		std::vector<VkImage> swapChainImages;

		/// swapchain images format 
		VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;

		/// default constructor
		ImageViewVkCreateInfo() = default;
	};
}

#endif