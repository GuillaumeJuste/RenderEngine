#pragma once

#ifndef RENDERENGINE_IMAGEVKCREATEINFO
#define RENDERENGINE_IMAGEVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for ImageView Initialization
	*/
	struct ImageVkCreateInfo
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		uint32_t width;
		uint32_t height;
		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkMemoryPropertyFlags properties;
		VkQueue graphicsQueue;
		CommandPool* commandPool;
		VkImageCreateFlags imageFlags;
		uint32_t arrayLayers;

		/// default constructor
		ImageVkCreateInfo() = default;
	};
}

#endif