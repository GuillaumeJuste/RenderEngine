#pragma once

#ifndef RENDERENGINE_FRAMEBUFFERVKCREATEINFO
#define RENDERENGINE_FRAMEBUFFERVKCREATEINFO

#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/DepthBuffer/DepthBuffer.hpp"

#include <vector>

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for FrameBuffer Initialization
	*/
	struct FrameBufferVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;
		
		/// render pass
		RenderPass* renderPass;
		
		/// image view
		std::vector<VkImageView> imageViews;
		
		/// swapchain image count
		size_t swapChainImageCount;
		
		/// swapchain extent
		VkExtent2D swapChainExtent;

		DepthBuffer* depthBuffer;

		Image* colorImage;

		/// default constructor
		FrameBufferVkCreateInfo() = default;
	};
}

#endif