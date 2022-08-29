#pragma once

#ifndef RENDERENGINE_FRAMEBUFFERCREATEINFO
#define RENDERENGINE_FRAMEBUFFERCREATEINFO

#include "Vulkan/RenderPass/RenderPass.hpp"
#include "Vulkan/ImageView/ImageView.hpp"

namespace RenderEngine::Vulkan
{
	struct FrameBufferCreateInfo
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
		FrameBufferCreateInfo() = default;
	};
}

#endif