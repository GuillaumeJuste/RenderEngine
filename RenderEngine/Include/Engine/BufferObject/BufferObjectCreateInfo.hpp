#pragma once

#ifndef RENDERENGINE_BUFFEROBJECTCREATEINFO
#define RENDERENGINE_BUFFEROBJECTCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	struct BufferObjectCreateInfo
	{
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice;
		VkDeviceSize bufferSize;
		VkBufferUsageFlags usage;
		VkMemoryPropertyFlags memoryProperties;

		BufferObjectCreateInfo() = default;
	};
}

#endif