#pragma once

#ifndef RENDERENGINE_VKTEXURECREATEINFO
#define RENDERENGINE_VKTEXURECREATEINFO

#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"

namespace RenderEngine::Engine::Vulkan
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

		std::vector<RenderEngine::Assets::Texture*> textures;

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