#pragma once

#ifndef RENDERENGINE_RENDERPASSCREATEINFO
#define RENDERENGINE_RENDERPASSCREATEINFO

#include "Vulkan/Shader/Shader.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for RenderPass Initialization
	*/
	struct RenderPassCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;

		/// swapchain image format
		VkFormat swapChainImageFormat;

		RenderPassCreateInfo() = default;
	};
}

#endif