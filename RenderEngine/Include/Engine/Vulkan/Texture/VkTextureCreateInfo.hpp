#pragma once

#ifndef RENDERENGINE_VKTEXURECREATEINFO
#define RENDERENGINE_VKTEXURECREATEINFO

#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "Core/RessourceManager/Texture.hpp"

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

		std::vector<RenderEngine::Core::Texture*> textures;

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