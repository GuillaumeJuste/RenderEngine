#include "Engine/Vulkan/CommandBuffer/Base/CommandBufferBase.hpp"
#include "Engine/Vulkan/CommandBuffer/Base/CommandBufferVkCreateInfo.hpp"

#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

#include <iostream>

using namespace RenderEngine::Engine::Vulkan;

void CommandBufferBase::InitializeCommandBuffer(const CommandBufferVkCreateInfo& _createInfo, CommandBufferBase* _output)
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

const VkCommandBuffer& CommandBufferBase::GetVKCommandBuffer() const
{
	return commandBuffer;
}