#pragma once

#ifndef RENDERENGINE_VKTEXURECREATEINFO
#define RENDERENGINE_VKTEXURECREATEINFO

#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"
#include "ResourceManager/Assets/Texture/RawTexture.hpp"

namespace RenderEngine::Rendering
{
	struct VkTextureVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;

		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// graphics queue
		VkQueue graphicsQueue;

		/// command pool
		CommandPool* commandPool;

		RenderEngine::Assets::RawTexture texture;

		uint32_t width;
		uint32_t height;
		uint32_t imageCount;
		uint32_t mipLevels;

		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlags usage; 
		VkMemoryPropertyFlags properties;

		VkImageCreateFlags imageFlags;

		VkImageViewType imageViewType;

		bool generateMipmap = true;

		/// default constructor
		VkTextureVkCreateInfo() = default;
	};
}

#endif