#pragma once

#ifndef RENDERENGINE_SWAPCHAINSUPPORTDETAILS
#define RENDERENGINE_SWAPCHAINSUPPORTDETAILS

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
}

#endif