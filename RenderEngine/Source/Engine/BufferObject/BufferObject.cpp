#include "Engine/BufferObject/BufferObject.hpp"

#include <iostream>

using namespace RenderEngine::Vulkan;

void BufferObject::InitializeBufferObject(BufferObjectCreateInfo _createInfo, BufferObject* _output)
{
	_output->physicalDevice = _createInfo.physicalDevice;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->bufferSize = _createInfo.bufferSize;
	_output->usage = _createInfo.usage;
	_output->memoryProperties = _createInfo.memoryProperties;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = _createInfo.bufferSize;
	bufferInfo.usage = _createInfo.usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(_createInfo.logicalDevice, &bufferInfo, nullptr, &_output->buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_createInfo.logicalDevice, _output->buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _output->FindMemoryType(memRequirements.memoryTypeBits, _createInfo.memoryProperties);

	if (vkAllocateMemory(_createInfo.logicalDevice, &allocInfo, nullptr, &_output->bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(_output->logicalDevice, _output->buffer, _output->bufferMemory, 0);
}

void BufferObject::CopyBuffer(BufferObject* _dstBuffer, CommandPool* _commandPool, VkQueue _queue, VkDeviceSize _size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool->GetCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.size = _size;
	vkCmdCopyBuffer(commandBuffer, buffer, _dstBuffer->GetVkBuffer(), 1, &copyRegion);
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_queue);

	vkFreeCommandBuffers(logicalDevice, _commandPool->GetCommandPool(), 1, &commandBuffer);
}

uint32_t BufferObject::FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties) {
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

const VkBuffer& BufferObject::GetVkBuffer() const
{
	return buffer;
}

const VkDeviceMemory& BufferObject::GetVkBufferMemory() const
{
	return bufferMemory;
}

const VkDeviceSize& BufferObject::GetBufferSize() const
{
	return bufferSize;
}

void BufferObject::Cleanup()
{
	vkDestroyBuffer(logicalDevice, buffer, nullptr);
	vkFreeMemory(logicalDevice, bufferMemory, nullptr);

	std::cout << "[Cleaned] Buffer Object" << std::endl;
}