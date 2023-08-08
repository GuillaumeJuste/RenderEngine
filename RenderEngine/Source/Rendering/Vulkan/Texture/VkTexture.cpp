#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"

#include <iostream>


using namespace RenderEngine::Rendering;

void VkTexture::InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output)
{
	_output->FillDataFromCreateInfo(_vkTextureCreateInfo);

	VkImageBufferCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	imageCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(_vkTextureCreateInfo.width);
	imageCreateInfo.height = static_cast<uint32_t>(_vkTextureCreateInfo.height);
	imageCreateInfo.format = _vkTextureCreateInfo.format;
	imageCreateInfo.tiling = _vkTextureCreateInfo.tiling;
	imageCreateInfo.usage = _vkTextureCreateInfo.usage;
	imageCreateInfo.properties = _vkTextureCreateInfo.properties;
	imageCreateInfo.commandPool = _vkTextureCreateInfo.commandPool;
	imageCreateInfo.graphicsQueue = _vkTextureCreateInfo.graphicsQueue;
	imageCreateInfo.textureCount = _vkTextureCreateInfo.imageCount;
	imageCreateInfo.imageFlags = _vkTextureCreateInfo.imageFlags;
	imageCreateInfo.imageViewType = _vkTextureCreateInfo.imageViewType;
	imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageCreateInfo.mipLevels = _vkTextureCreateInfo.mipLevels;
	VkImageBuffer::InitializeImageBuffer(imageCreateInfo, &_output->imageBuffer);

	_output->CreateSampler(_vkTextureCreateInfo.mipLevels);

}

void VkTexture::FillDataFromCreateInfo(const VkTextureVkCreateInfo& _vkTextureCreateInfo)
{
	elementSize = _vkTextureCreateInfo.channels;
	if (_vkTextureCreateInfo.format == VK_FORMAT_R32G32B32A32_SFLOAT)
		elementSize *= 4;

	logicalDevice = _vkTextureCreateInfo.logicalDevice;

	physicalDevice = _vkTextureCreateInfo.physicalDevice;

	graphicsQueue = _vkTextureCreateInfo.graphicsQueue;

	commandPool = _vkTextureCreateInfo.commandPool;
}

void VkTexture::CreateSampler(uint32_t _mipmap)
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(_mipmap);

	if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VkTexture::Clean()
{
	vkDestroySampler(logicalDevice, sampler, nullptr);
	imageBuffer.Cleanup();
}


VkImageBuffer* VkTexture::GetImageBuffer()
{
	return &imageBuffer;
}

VkImageView VkTexture::GetImageView() const
{
	return imageBuffer.GetImageView();
}

VkSampler VkTexture::GetSampler() const
{
	return sampler;
}

void VkTexture::GetTextureData(char* _output, uint32_t _imageSize)
{
	BufferObject stagingBuffer;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = _imageSize;

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

	imageBuffer.CopyImageToBuffer(stagingBuffer.GetVkBuffer(), elementSize);

	void* data;
	vkMapMemory(logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
	memcpy(_output, data, stagingBuffer.GetBufferSize());
	vkUnmapMemory(logicalDevice, stagingBuffer.GetVkBufferMemory());

	stagingBuffer.Clean();
}