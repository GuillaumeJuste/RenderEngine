#pragma once

#ifndef RENDERENGINE_FRAMEBUFFERCREATEINFO
#define RENDERENGINE_FRAMEBUFFERCREATEINFO

#include "Engine/RenderPass/RenderPass.hpp"
#include "Engine/ImageView/ImageView.hpp"

namespace RenderEngine::Vulkan
{
	struct FrameBufferCreateInfo
	{
		VkDevice logicalDevice;
		RenderPass* renderPass;
		const ImageView* imageView;
		size_t swapChainImageCount;
		VkExtent2D swapChainExtent;

		FrameBufferCreateInfo() = default;
	};
}

#endif