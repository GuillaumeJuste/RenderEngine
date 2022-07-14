#include "Engine/ImageView/ImageView.hpp"

#include <stdexcept>
#include <iostream>

#include "Engine/ImageView/ImageViewCreateInfo.hpp"

using namespace RenderEngine;

void ImageView::InitializeImageView(ImageViewCreateInfo _imageViewCreateInfo, ImageView* _output)
{
	_output->logicalDevice = _imageViewCreateInfo.logicalDevice;

	_output->swapChainImageViews.resize(_imageViewCreateInfo.swapChainImageCount);
	for (size_t i = 0; i < _imageViewCreateInfo.swapChainImageCount; i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _imageViewCreateInfo.swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _imageViewCreateInfo.swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(*_imageViewCreateInfo.logicalDevice, &createInfo, nullptr, &(_output->swapChainImageViews[i])) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

void ImageView::Cleanup()
{
	std::cout << "[Cleaning] Image View" << std::endl;
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(*logicalDevice, imageView, nullptr);
	}
	std::cout << "[Cleaned] Image View" << std::endl;
}

const std::vector<VkImageView>& ImageView::GetSwapChainImageViews() const
{
	return swapChainImageViews;
}