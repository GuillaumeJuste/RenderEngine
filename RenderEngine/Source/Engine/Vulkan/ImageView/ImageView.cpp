#include "Engine/Vulkan/ImageView/ImageView.hpp"

#include <stdexcept>
#include <iostream>

#include "Engine/Vulkan/ImageView/ImageViewVkCreateInfo.hpp"

using namespace RenderEngine::Engine::Vulkan;

void ImageView::InitializeImageView(ImageViewVkCreateInfo _imageViewCreateInfo, ImageView* _output)
{
	_output->logicalDevice = _imageViewCreateInfo.logicalDevice;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = _imageViewCreateInfo.image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = _imageViewCreateInfo.format;
    viewInfo.subresourceRange.aspectMask = _imageViewCreateInfo.aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    ;
    if (vkCreateImageView(_imageViewCreateInfo.logicalDevice, &viewInfo, nullptr, &_output->imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void ImageView::Cleanup()
{
	vkDestroyImageView(logicalDevice, imageView, nullptr);
	std::cout << "[Cleaned] Image View" << std::endl;
}

const VkImageView& ImageView::GetImageView() const
{
	return imageView;
}