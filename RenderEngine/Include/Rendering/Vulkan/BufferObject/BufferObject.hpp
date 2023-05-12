#pragma once

#ifndef RENDERENGINE_BUFFEROBJECT
#define RENDERENGINE_BUFFEROBJECT

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObjectVkCreateInfo.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"
#include "Rendering/Base/Interface/Primitive/IBuffer.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Buffer Object class
	 * Hold data that will be sent to the GPU for computation and/or render
	*/
	class BufferObject : public IBuffer
	{
	private:
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
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		/**
		 * @brief memoery property flags
		*/
		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

		/**
		 * @brief vkBuffer handle
		*/
		VkBuffer buffer = VK_NULL_HANDLE;

		/**
		 * @brief buffer memory handle
		*/
		VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

	public:
		BufferObject() = default;
		~BufferObject() = default;

		/**
		 * @brief Initialize buffer object from createinfo
		 * @param _createInfo information to create BufferObject from.
		 * @param _output BufferObject to initialize
		*/
		static void InitializeBufferObject(BufferObjectVkCreateInfo _createInfo, BufferObject* _output);

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
		void Clean();
	};

}

#endif