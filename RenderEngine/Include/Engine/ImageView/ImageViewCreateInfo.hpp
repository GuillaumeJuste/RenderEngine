#pragma once

#ifndef RENDERENGINE_IMAGEVIEWCREATEINFO
#define RENDERENGINE_IMAGEVIEWCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Surface.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	struct ImageViewCreateInfo
	{
		VkDevice logicalDevice = VK_NULL_HANDLE;
		size_t swapChainImageCount = 2;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;

		ImageViewCreateInfo() = default;
	};
}

#endif