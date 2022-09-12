#pragma once

#ifndef RENDERENGINE_RENDERPASSVKCREATEINFO
#define RENDERENGINE_RENDERPASSVKCREATEINFO

#include "Engine/Vulkan/Shader/Shader.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for RenderPass Initialization
	*/
	struct RenderPassVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;

		/// swapchain image format
		VkFormat swapChainImageFormat;

		RenderPassVkCreateInfo() = default;
	};
}

#endif