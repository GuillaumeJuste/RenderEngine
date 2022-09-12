#pragma once

#ifndef RENDERENGINE_SWAPCHAINSUPPORTDETAILS
#define RENDERENGINE_SWAPCHAINSUPPORTDETAILS

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief struct holding a supported swapchain features
	*/
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
}

#endif