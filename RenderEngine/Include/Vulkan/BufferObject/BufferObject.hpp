#pragma once

#ifndef RENDERENGINE_BUFFEROBJECT
#define RENDERENGINE_BUFFEROBJECT

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/BufferObject/BufferObjectCreateInfo.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Buffer Object class
	*/
	class BufferObject
	{
	private:
		/**
		 * @brief vulkan physical device handle
		*/
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/**
		 * @brief vulkan logical device handle
		*/
		VkDevice logicalDevice;

		/**
		 * @brief Size of the buffer to be allocated
		*/
		VkDeviceSize bufferSize;

		/**
		 * @brief buffer usage flags
		*/
		VkBufferUsageFlags usage;

		/**
		 * @brief memoery property flags
		*/
		VkMemoryPropertyFlags memoryProperties;

		/**
		 * @brief vkBuffer handle
		*/
		VkBuffer buffer;

		/**
		 * @brief buffer memory handle
		*/
		VkDeviceMemory bufferMemory;

		/**
		 * @brief find memory type usable for buffer memory
		 * @param _typeFilter mask corresponding to all memory type supported by the physical device
		 * @param _properties requiered memory property
		 * @return index of memory matching properties
		*/
		uint32_t FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties);
	public:
		BufferObject() = default;
		~BufferObject() = default;

		/**
		 * @brief Initialize buffer object from createinfo
		 * @param _createInfo information to create BufferObject from.
		 * @param _output BufferObject to initialize
		*/
		static void InitializeBufferObject(BufferObjectCreateInfo _createInfo, BufferObject* _output);

		/**
		 * @brief copy this buffer into destination buffer; 
		 * @param _dstBuffer destination buffer to copy this buffer to
		 * @param _commandPool command pool to use to copy buffer
		 * @param _queue queue to use to copy buffer
		 * @param _size buffer size
		*/
		void CopyBuffer(BufferObject* _dstBuffer, CommandPool* _commandPool, VkQueue _queue, VkDeviceSize _size);

		/**
		 * @brief Get Buffer
		 * @return VkBuffer handle
		*/
		const VkBuffer& GetVkBuffer() const;

		/**
		 * @brief Get Buffer memory
		 * @return VkBufferMemory handle
		*/
		const VkDeviceMemory& GetVkBufferMemory() const;

		/**
		 * @brief Get Buffer size
		 * @return buffer size;
		*/
		const VkDeviceSize& GetBufferSize() const;

		/**
		 * @brief cleanup function
		*/
		void Cleanup();
	};

}

#endif