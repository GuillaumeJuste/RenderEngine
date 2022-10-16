#include "Engine/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Engine/Vulkan/CommandBuffer/CommandBufferVkCreateInfo.hpp"

#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

#include <iostream>

using namespace RenderEngine::Engine::Vulkan;

void CommandBuffer::InitializeCommandBuffer(const CommandBufferVkCreateInfo& _createInfo, CommandBuffer* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->commandPool = _createInfo.commandPool;

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _createInfo.commandPool->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(_createInfo.logicalDevice, &allocInfo, &_output->commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

VkCommandBuffer CommandBuffer::BeginSingleTimeCommands(VkDevice _logicalDevice, CommandPool* _commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool->GetCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void CommandBuffer::EndSingleTimeCommands(VkDevice _logicalDevice, CommandPool* _commandPool, VkQueue _queue, VkCommandBuffer _commandBuffer)
{
	vkEndCommandBuffer(_commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffer;

	vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_queue);

	vkFreeCommandBuffers(_logicalDevice, _commandPool->GetCommandPool(), 1, &_commandBuffer);
}

const VkCommandBuffer& CommandBuffer::GetVKCommandBuffer() const
{
	return commandBuffer;
}