#pragma once

#ifndef RENDERENGINE_BUFFEROBJECTCREATEINFO
#define RENDERENGINE_BUFFEROBJECTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief struct holding data for BufferObject creation
	*/
	struct BufferObjectCreateInfo
	{
		/**
		 * @brief vulkan physical device handle
		*/
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/**
		 * @brief vulkan logical device handle
		*/
		VkDevice logicalDevice = VK_NULL_HANDLE;

		/**
		 * @brief Size of the buffer to be allocated
		*/
		VkDeviceSize bufferSize = 0;

		/**
		 * @brief buffer usage flags
		*/
		VkBufferUsageFlags usage;

		/**
		 * @brief memoery property flags
		*/
		VkMemoryPropertyFlags memoryProperties;

		BufferObjectCreateInfo() = default;
	};
}

#endif