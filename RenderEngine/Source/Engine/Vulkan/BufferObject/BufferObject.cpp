#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Engine/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Engine/Vulkan/Misc/Utils.hpp"
#include <iostream>

using namespace RenderEngine::Engine::Vulkan;

void BufferObject::InitializeBufferObject(BufferObjectVkCreateInfo _createInfo, BufferObject* _output)
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
	allocInfo.memoryTypeIndex = FindMemoryType(_output->physicalDevice, memRequirements.memoryTypeBits, _createInfo.memoryProperties);

	if (vkAllocateMemory(_createInfo.logicalDevice, &allocInfo, nullptr, &_output->bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(_output->logicalDevice, _output->buffer, _output->bufferMemory, 0);
}

void BufferObject::CopyBuffer(BufferObject* _dstBuffer, CommandPool* _commandPool, VkQueue _queue, VkDeviceSize _size)
{
	VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, _commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.size = _size;
	vkCmdCopyBuffer(commandBuffer, buffer, _dstBuffer->GetVkBuffer(), 1, &copyRegion);
	
	CommandBuffer::EndSingleTimeCommands(logicalDevice, _commandPool, _queue, commandBuffer);
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