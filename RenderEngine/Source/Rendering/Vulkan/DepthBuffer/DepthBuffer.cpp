#include "Rendering/Vulkan/DepthBuffer/DepthBuffer.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"

#include <iostream>

using namespace RenderEngine::Rendering;

void DepthBuffer::InitializeDepthBuffer(const DepthBufferVkCreateInfo& _createInfo, DepthBuffer* _output)
{
    _output->physicalDevice = _createInfo.physicalDevice;
    _output->logicalDevice = _createInfo.logicalDevice;

    VkFormat depthFormat = _output->FindDepthFormat();

    ImageVkCreateInfo imageCreateInfo{};
    imageCreateInfo.physicalDevice = _createInfo.physicalDevice;
    imageCreateInfo.logicalDevice = _createInfo.logicalDevice;
    imageCreateInfo.width = _createInfo.swapChainExtent.width;
    imageCreateInfo.height = _createInfo.swapChainExtent.height;
    imageCreateInfo.format = depthFormat;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    imageCreateInfo.commandPool = _createInfo.commandPool;
    imageCreateInfo.graphicsQueue = _createInfo.graphicsQueue;
    imageCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;
    imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
    imageCreateInfo.arrayLayers = 1;

    Image::InitializeImage(imageCreateInfo, &_output->depthImage);

    _output->depthImage.TransitionImageLayout(depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

}

VkFormat DepthBuffer::FindSupportedFormat(const std::vector<VkFormat>& _candidates, VkImageTiling _tiling, VkFormatFeatureFlags _features)
{
    for (VkFormat format : _candidates) 
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (_tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _features) == _features) 
        {
            return format;
        }
        else if (_tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _features) == _features) 
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat DepthBuffer::FindDepthFormat()
{
    return FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void DepthBuffer::Cleanup()
{
    depthImage.Cleanup();
}

const VkImage& DepthBuffer::GetVkImage() const
{
    return depthImage.GetVkImage();
}

const VkImageView& DepthBuffer::GetVkImageView() const
{
    return depthImage.GetImageView();
}