#pragma once

#ifndef RENDERENGINE_FRAMEBUFFERVKCREATEINFO
#define RENDERENGINE_FRAMEBUFFERVKCREATEINFO

#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/ImageView/ImageView.hpp"
#include "Engine/Vulkan/DepthBuffer/DepthBuffer.hpp"

#include <vector>

namespace RenderEngine::Engine::Vulkan
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
		std::vector<ImageView> imageViews;
		
		/// swapchain image count
		size_t swapChainImageCount;
		
		/// swapchain extent
		VkExtent2D swapChainExtent;

		DepthBuffer* depthBuffer;

		/// default constructor
		FrameBufferVkCreateInfo() = default;
	};
}

#endif