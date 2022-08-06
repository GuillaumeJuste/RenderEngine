#pragma once

#ifndef RENDERENGINE_SWAPCHAINSUPPORTDETAILS
#define RENDERENGINE_SWAPCHAINSUPPORTDETAILS

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
}

#endif