#pragma once

#ifndef RENDERENGINE_SWAPCHAINCREATEINFO
#define RENDERENGINE_SWAPCHAINCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Surface.hpp"

namespace RenderEngine
{
	class Window;

	struct SwapChainCreateInfo
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		Surface surface;
		Window* window;

		SwapChainCreateInfo() = default;
	};
}

#endif