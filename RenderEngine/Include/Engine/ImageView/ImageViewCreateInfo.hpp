#pragma once

#ifndef RENDERENGINE_IMAGEVIEWCREATEINFO
#define RENDERENGINE_IMAGEVIEWCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Surface.hpp"

namespace RenderEngine
{
	class Window;

	struct ImageViewCreateInfo
	{
		VkDevice logicalDevice;
		size_t swapChainImageCount;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;

		ImageViewCreateInfo() = default;
	};
}

#endif