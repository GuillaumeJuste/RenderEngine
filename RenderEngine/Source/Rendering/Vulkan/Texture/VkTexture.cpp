#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"

#include <iostream>


using namespace RenderEngine::Rendering;

void VkTexture::InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output)
{
	_output->createInfo = _vkTextureCreateInfo;

	BufferObject stagingBuffer;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = _vkTextureCreateInfo.texture.imageSize * _vkTextureCreateInfo.texture.textureCount;

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

	void* data;
	vkMapMemory(_vkTextureCreateInfo.logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
	memcpy(data, _vkTextureCreateInfo.texture.pixels, stagingBuffer.GetBufferSize());
	vkUnmapMemory(_vkTextureCreateInfo.logicalDevice, stagingBuffer.GetVkBufferMemory());

	ImageVkCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = _vkTextureCreateInfo.physicalDevice;
	imageCreateInfo.logicalDevice = _vkTextureCreateInfo.logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(_vkTextureCreateInfo.texture.width);
	imageCreateInfo.height = static_cast<uint32_t>(_vkTextureCreateInfo.texture.height);
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = _vkTextureCreateInfo.usage;
	imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	imageCreateInfo.commandPool = _vkTextureCreateInfo.commandPool;
	imageCreateInfo.graphicsQueue = _vkTextureCreateInfo.graphicsQueue;
	imageCreateInfo.textureCount = _vkTextureCreateInfo.texture.textureCount;
	imageCreateInfo.imageFlags = _vkTextureCreateInfo.imageFlags;
	imageCreateInfo.imageViewType = _vkTextureCreateInfo.imageViewType;
	imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageCreateInfo.mipLevels = _vkTextureCreateInfo.texture.mipLevels;
	Image::InitializeImage(imageCreateInfo, &_output->image);

	_output->image.TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	_output->image.CopyBufferToImage(stagingBuffer.GetVkBuffer());

	if(_vkTextureCreateInfo.texture.mipLevels > 1)
		_output->image.GenerateMipmaps(VK_FORMAT_R8G8B8A8_SRGB);
	else
		_output->image.TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Clean();

	_output->CreateSampler();
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
	samplerInfo.maxLod = static_cast<float>(createInfo.texture.mipLevels);

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

VkImageView VkTexture::GetImageView() const
{
	return image.GetImageView();
}

VkSampler VkTexture::GetSampler() const
{
	return sampler;
}