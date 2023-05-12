#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"

#include <iostream>

using namespace RenderEngine::Rendering;

void CommandPool::InitializeCommandPool(const CommandPoolVkCreateInfo& _createInfo, CommandPool* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->graphicsQueueIndex = _createInfo.graphicsQueueIndex;

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = _createInfo.graphicsQueueIndex;

	if (vkCreateCommandPool(_createInfo.logicalDevice, &poolInfo, nullptr, &_output->commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

const VkCommandPool& CommandPool::GetCommandPool() const
{
	return commandPool;
}

void CommandPool::Cleanup()
{
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}