#pragma once

#ifndef RENDERENGINE_RENDERPASSVKCREATEINFO
#define RENDERENGINE_RENDERPASSVKCREATEINFO

#include "Rendering/Vulkan/DepthBuffer/DepthBuffer.hpp"

namespace RenderEngine::Rendering
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

		DepthBuffer* depthBuffer;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		RenderPassVkCreateInfo() = default;
	};
}

#endif