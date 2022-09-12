#pragma once

#ifndef RENDERENGINE_FRAMEBUFFERVKCREATEINFO
#define RENDERENGINE_FRAMEBUFFERVKCREATEINFO

#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/ImageView/ImageView.hpp"

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
		const ImageView* imageView;
		
		/// swapchain image count
		size_t swapChainImageCount;
		
		/// swapchain extent
		VkExtent2D swapChainExtent;

		/// default constructor
		FrameBufferVkCreateInfo() = default;
	};
}

#endif