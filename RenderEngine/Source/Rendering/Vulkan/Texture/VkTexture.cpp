#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"
#include "ResourceManager/Assets/Texture/RawTexture.hpp"

#include <iostream>


using namespace RenderEngine::Rendering;

void VkTexture::InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output, bool _fillImage)
{
	_output->createInfo = _vkTextureCreateInfo;

	ImageVkCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	imageCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(_vkTextureCreateInfo.width);
	imageCreateInfo.height = static_cast<uint32_t>(_vkTextureCreateInfo.height);
	imageCreateInfo.format = _vkTextureCreateInfo.format;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = _vkTextureCreateInfo.usage;
	imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	imageCreateInfo.commandPool = _vkTextureCreateInfo.commandPool;
	imageCreateInfo.graphicsQueue = _vkTextureCreateInfo.graphicsQueue;
	imageCreateInfo.textureCount = _vkTextureCreateInfo.imageCount;
	imageCreateInfo.imageFlags = _vkTextureCreateInfo.imageFlags;
	imageCreateInfo.imageViewType = _vkTextureCreateInfo.imageViewType;
	imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageCreateInfo.mipLevels = _vkTextureCreateInfo.mipLevels;
	Image::InitializeImage(imageCreateInfo, &_output->image);

	if (_fillImage)
	{
		if (_vkTextureCreateInfo.texture.isHdr == false)
			_output->FillImageBuffer<char>(_vkTextureCreateInfo.texture.dataC);
		else
			_output->FillImageBuffer<float>(_vkTextureCreateInfo.texture.dataF);

	}
	_output->CreateSampler(_vkTextureCreateInfo.mipLevels);

}

void VkTexture::CreateSampler(uint32_t _mipmap)
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(createInfo.physicalDevice, &properties);

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

	if (vkCreateSampler(createInfo.logicalDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VkTexture::Clean()
{
	vkDestroySampler(createInfo.logicalDevice, sampler, nullptr);
	image.Cleanup();
}


Image* VkTexture::GetImage()
{
	return &image;
}

VkImageView VkTexture::GetImageView() const
{
	return image.GetImageView();
}

VkSampler VkTexture::GetSampler() const
{
	return sampler;
}

void VkTexture::GetTextureData(char* _output, uint32_t _imageSize)
{
	BufferObject stagingBuffer;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = _imageSize;
	if (createInfo.format == VK_FORMAT_R32G32B32A32_SFLOAT)
		stagingBufferCreateInfo.bufferSize *= sizeof(float);

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

	image.CopyImageToBuffer(stagingBuffer.GetVkBuffer());

	void* data;
	vkMapMemory(createInfo.logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
	memcpy(_output, data, stagingBuffer.GetBufferSize());
	vkUnmapMemory(createInfo.logicalDevice, stagingBuffer.GetVkBufferMemory());

	stagingBuffer.Clean();
}