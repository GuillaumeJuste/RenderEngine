#pragma once

#ifndef RENDERENGINE_BUFFEROBJECT
#define RENDERENGINE_BUFFEROBJECT

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/BufferObject/BufferObjectCreateInfo.hpp"
#include "Engine/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{

	class BufferObject
	{
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		VkDeviceSize bufferSize;
		VkBufferUsageFlags usage;
		VkMemoryPropertyFlags memoryProperties;

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;

		uint32_t FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties);
	public:
		BufferObject() = default;
		~BufferObject() = default;

		static void InitializeBufferObject(BufferObjectCreateInfo _createInfo, BufferObject* _output);

		void CopyBuffer(BufferObject* _dstBuffer, CommandPool* _commandPool, VkQueue _queue, VkDeviceSize _size);

		const VkBuffer& GetVkBuffer() const;

		const VkDeviceMemory& GetVkBufferMemory() const;

		const VkDeviceSize& GetBufferSize() const;

		void Cleanup();
	};

}

#endif