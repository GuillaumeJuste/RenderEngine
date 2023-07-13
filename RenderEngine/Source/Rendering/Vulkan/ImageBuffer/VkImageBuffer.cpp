#include "Rendering/Vulkan/ImageBuffer/VkImageBuffer.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"
#include <iostream>

using namespace RenderEngine::Rendering;

void VkImageBuffer::InitializeImageBuffer(const VkImageBufferCreateInfo& _imageCreateInfo, VkImageBuffer* _output)
{
    _output->FillDataFromCreateInfo(_imageCreateInfo);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = _imageCreateInfo.width;
    imageInfo.extent.height = _imageCreateInfo.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = _imageCreateInfo.mipLevels;
    imageInfo.arrayLayers = _imageCreateInfo.textureCount > 0 ? _imageCreateInfo.textureCount : 1;
    imageInfo.format = _imageCreateInfo.format;
    imageInfo.tiling = _imageCreateInfo.tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = _imageCreateInfo.usage;
    imageInfo.samples = _imageCreateInfo.samples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = _imageCreateInfo.imageFlags;

    if (vkCreateImage(_imageCreateInfo.logicalDevice, &imageInfo, nullptr, &_output->image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_imageCreateInfo.logicalDevice, _output->image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(_imageCreateInfo.physicalDevice, memRequirements.memoryTypeBits, _imageCreateInfo.properties);

    if (vkAllocateMemory(_imageCreateInfo.logicalDevice, &allocInfo, nullptr, &_output->imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_imageCreateInfo.logicalDevice, _output->image, _output->imageMemory, 0);

    _output->CreateImageView(_imageCreateInfo.imageViewType, _imageCreateInfo.imageViewAspectFlags);
    _output->currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

void VkImageBuffer::FillDataFromCreateInfo(const VkImageBufferCreateInfo& _imageCreateInfo)
{
    physicalDevice = _imageCreateInfo.physicalDevice;
    logicalDevice = _imageCreateInfo.logicalDevice;
    width = _imageCreateInfo.width;
    height = _imageCreateInfo.height;
    textureCount = _imageCreateInfo.textureCount;
    mipLevels = _imageCreateInfo.mipLevels;

    format = _imageCreateInfo.format;
    graphicsQueue = _imageCreateInfo.graphicsQueue;
    commandPool = _imageCreateInfo.commandPool;
}

void VkImageBuffer::TransitionImageLayout(VkImageLayout _newLayout, VkCommandBuffer _commandBuffer)
{
    VkCommandBuffer commandBuffer = _commandBuffer;
    if(commandBuffer == nullptr)
        commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = currentLayout;
    barrier.newLayout = _newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = textureCount;

    VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

    if (_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (HasStencilComponent(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }

    switch (currentLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        // Image layout is undefined (or does not matter)
        // Only valid as initial layout
        // No flags required, listed only for completeness
        barrier.srcAccessMask = 0;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        // Image is preinitialized
        // Only valid as initial layout for linear images, preserves memory contents
        // Make sure host writes have been finished
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        // Image is a color attachment
        // Make sure any writes to the color buffer have been finished
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        // Image is a depth/stencil attachment
        // Make sure any writes to the depth/stencil buffer have been finished
        barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        // Image is a transfer source
        // Make sure any reads from the image have been finished
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        // Image is a transfer destination
        // Make sure any writes to the image have been finished
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        // Image is read by a shader
        // Make sure any shader reads from the image have been finished
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        break;
    default:
        // Other source layouts aren't handled (yet)
        break;
    }

    // Target layouts (new)
            // Destination access mask controls the dependency for the new image layout
    switch (_newLayout)
    {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        // Image will be used as a transfer destination
        // Make sure any writes to the image have been finished
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        // Image will be used as a transfer source
        // Make sure any reads from the image have been finished
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        // Image will be used as a color attachment
        // Make sure any writes to the color buffer have been finished
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        // Image layout will be used as a depth/stencil attachment
        // Make sure any writes to depth/stencil buffer have been finished
        barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        // Image will be read in a shader (sampler, input attachment)
        // Make sure any writes to the image have been finished
        if (barrier.srcAccessMask == 0)
        {
            barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        break;
    default:
        // Other source layouts aren't handled (yet)
        break;
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    if (_commandBuffer == nullptr)
        CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);

    currentLayout = _newLayout;
}

bool VkImageBuffer::HasStencilComponent(VkFormat _format)
{
    return _format == VK_FORMAT_D32_SFLOAT_S8_UINT || _format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void VkImageBuffer::CreateImageView(VkImageViewType _imageViewType, VkImageAspectFlags _imageViewAspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.viewType = _imageViewType;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = _imageViewAspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = textureCount;
    viewInfo.image = image;

    if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void VkImageBuffer::CopyBufferToImage(VkBuffer _buffer, uint32_t _elementSize, VkCommandBuffer _commandBuffer, bool _copyMipmap)
{
    VkCommandBuffer commandBuffer = _commandBuffer;
    if (commandBuffer == nullptr)
        commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    uint32_t mipCount = 1;
    if (_copyMipmap)
        mipCount = mipLevels;

    for (uint32_t m = 0; m < mipCount; m++)
    {
        uint32_t mipWidth =  (uint32_t)((float)width * std::pow(0.5f, (float)m));
        uint32_t mipHeight = (uint32_t)((float)height * std::pow(0.5f, (float)m));

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = m;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = textureCount;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            mipWidth,
            mipHeight,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            _buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        region.bufferOffset += mipWidth * mipHeight * textureCount * _elementSize;
    }
    if (_commandBuffer == nullptr)
        CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);
}

bool VkImageBuffer::CopyImageToBuffer(VkBuffer _buffer, uint32_t _elementSize)
{
    VkImageLayout oldLayout = currentLayout;

    VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);

    TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandBuffer);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    for (uint32_t m = 0; m < mipLevels; m++)
    {
        uint32_t mipWidth = (uint32_t)((float)width * std::pow(0.5f, (float)m));
        uint32_t mipHeight = (uint32_t)((float)height * std::pow(0.5f, (float)m));

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = m;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = textureCount;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            mipWidth,
            mipHeight,
            1
        };

        vkCmdCopyImageToBuffer(
            commandBuffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            _buffer,
            1,
            &region
        );
        region.bufferOffset += mipWidth * mipHeight * textureCount * _elementSize;
    }
    
    TransitionImageLayout(oldLayout, commandBuffer);

    CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);

    return true;
}

void VkImageBuffer::GenerateMipmaps()
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) 
    {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = textureCount;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = width;
    int32_t mipHeight = height;

    for (uint32_t i = 1; i < mipLevels; i++) 
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = currentLayout;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = textureCount;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = textureCount;

        vkCmdBlitImage(commandBuffer,
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);
}

void VkImageBuffer::Cleanup()
{
    vkDestroyImageView(logicalDevice, imageView, nullptr);
    vkDestroyImage(logicalDevice, image, nullptr);
    vkFreeMemory(logicalDevice, imageMemory, nullptr);
}

const VkImage& VkImageBuffer::GetVkImage() const
{
    return image;
}

const VkDeviceMemory& VkImageBuffer::GetVkImageMemory() const
{
    return imageMemory;
}

const VkImageView& VkImageBuffer::GetImageView() const
{
    return imageView;
}