#pragma once

#ifndef RENDERENGINE_BUFFEROBJECTVKCREATEINFO
#define RENDERENGINE_BUFFEROBJECTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief struct holding data for BufferObject Initialization
	*/
	struct BufferObjectVkCreateInfo
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

		BufferObjectVkCreateInfo() = default;
	};
}

#endif