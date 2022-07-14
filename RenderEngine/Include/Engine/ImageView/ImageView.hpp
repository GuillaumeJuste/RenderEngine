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
		VkDevice* logicalDevice = VK_NULL_HANDLE;
		std::vector<VkImageView> swapChainImageViews;

	public:
		ImageView() = default;
		~ImageView() = default;

		static void InitializeImageView(ImageViewCreateInfo _imageViewCreateInfo, ImageView* _output);
		void Cleanup();

		const std::vector<VkImageView>& GetSwapChainImageViews();
	};

}

#endif