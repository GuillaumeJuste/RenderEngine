#pragma once

#ifndef RENDERENGINE_IMAGEVKCREATEINFO
#define RENDERENGINE_IMAGEVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for Image Initialization
	*/
	struct ImageVkCreateInfo
	{
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		uint32_t width;
		uint32_t height;
		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkMemoryPropertyFlags properties;
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		CommandPool* commandPool = nullptr;
		VkImageCreateFlags imageFlags;
		uint32_t textureCount;
		VkImageViewType imageViewType;
		VkImageAspectFlags imageViewAspectFlags;
		uint32_t mipLevels;
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
		/// default constructor
		ImageVkCreateInfo() = default;
	};
}

#endif