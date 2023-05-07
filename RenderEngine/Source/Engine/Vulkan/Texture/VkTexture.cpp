#include "Engine/Vulkan/Texture/VkTexture.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Engine/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Engine/Vulkan/Misc/Utils.hpp"

#include <iostream>


using namespace RenderEngine::Engine::Vulkan;

void VkTexture::InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output)
{
	_output->createInfo = _vkTextureCreateInfo;

	BufferObject stagingBuffer;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = _vkTextureCreateInfo.textures[0]->imageSize * _vkTextureCreateInfo.textures.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

	/*std::vector<char> images;
	for (size_t i = 0; i < _vkTextureCreateInfo.textures.size(); i++)
	{
		images.insert(images.end(), _vkTextureCreateInfo.textures[i]->pixels.begin(), _vkTextureCreateInfo.textures[i]->pixels.end());
	}*/

	void* data;
	vkMapMemory(_vkTextureCreateInfo.logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
	memcpy(data, _vkTextureCreateInfo.textures[0]->pixels, stagingBuffer.GetBufferSize());
	vkUnmapMemory(_vkTextureCreateInfo.logicalDevice, stagingBuffer.GetVkBufferMemory());

	ImageVkCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	imageCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(_vkTextureCreateInfo.textures[0]->width);
	imageCreateInfo.height = static_cast<uint32_t>(_vkTextureCreateInfo.textures[0]->height);
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = _vkTextureCreateInfo.usage;
	imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	imageCreateInfo.commandPool = _vkTextureCreateInfo.commandPool;
	imageCreateInfo.graphicsQueue = _vkTextureCreateInfo.graphicsQueue;
	imageCreateInfo.arrayLayers = _vkTextureCreateInfo.textures.size();
	imageCreateInfo.imageFlags = _vkTextureCreateInfo.imageFlags;
	Image::InitializeImage(imageCreateInfo, &_output->image);

	_output->image.TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	_output->CopyBufferToImage(stagingBuffer.GetVkBuffer(), static_cast<uint32_t>(imageCreateInfo.width), static_cast<uint32_t>(imageCreateInfo.height));
	_output->image.TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Cleanup();

	_output->CreateImageView(_vkTextureCreateInfo.imageViewType, _vkTextureCreateInfo.textures.size());

	_output->CreateSampler();
}

void VkTexture::CopyBufferToImage(VkBuffer _buffer, uint32_t _width, uint32_t _height)
{
	VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(createInfo.logicalDevice, createInfo.commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		_width,
		_height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		_buffer,
		image.GetVkImage(),
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	CommandBuffer::EndSingleTimeCommands(createInfo.logicalDevice, createInfo.commandPool, createInfo.graphicsQueue, commandBuffer);
}

void VkTexture::CreateImageView(VkImageViewType _imageViewType, uint32_t _layerCount)
{
	ImageViewVkCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.logicalDevice = createInfo.logicalDevice;
	imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageViewCreateInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.image = image.GetVkImage();
	imageViewCreateInfo.layerCount = _layerCount;
	imageViewCreateInfo.viewType = _imageViewType;
	ImageView::InitializeImageView(imageViewCreateInfo, &imageView);
}

void VkTexture::CreateSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(createInfo.physicalDevice, &properties);

	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(createInfo.logicalDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VkTexture::Cleanup()
{
	vkDestroySampler(createInfo.logicalDevice, sampler, nullptr);
	imageView.Cleanup();
	image.Cleanup();
}

VkImageView VkTexture::GetImageView() const
{
	return imageView.GetImageView();
}

VkSampler VkTexture::GetSampler() const
{
	return sampler;
}