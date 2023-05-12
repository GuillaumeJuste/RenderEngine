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

		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlags usage; 
		VkMemoryPropertyFlags properties;

		VkImageCreateFlags imageFlags;

		VkImageViewType imageViewType;

		/// default constructor
		VkTextureVkCreateInfo() = default;
	};
}

#endif