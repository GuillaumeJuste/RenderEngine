#pragma once

#ifndef RENDERENGINE_IMAGEVIEW
#define RENDERENGINE_IMAGEVIEW

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	struct ImageViewCreateInfo;

	class ImageView
	{
	private:
		VkDevice logicalDevice;
		std::vector<VkImageView> swapChainImageViews;

	public:
		ImageView() = default;
		~ImageView() = default;

		void InitializeImageView(ImageViewCreateInfo _imageViewCreateInfo);
		void Cleanup();

		const std::vector<VkImageView>& GetSwapChainImageViews();
	};

}

#endif