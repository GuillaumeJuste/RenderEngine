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

		RenderEngine::Core::Texture* texture;

		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlags usage; 
		VkMemoryPropertyFlags properties;

		/// default constructor
		VkTextureVkCreateInfo() = default;
	};
}

#endif