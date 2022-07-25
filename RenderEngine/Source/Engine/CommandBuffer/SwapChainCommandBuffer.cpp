#include "Engine/CommandBuffer/SwapChainCommandBuffer.hpp"

#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/RenderPass/RenderPass.hpp"
#include "Engine/FrameBuffer/FrameBuffer.hpp"

#include <iostream>

using namespace RenderEngine;

void SwapChainCommandBuffer::InitializeCommandBuffer(CommandBufferCreateInfo _createInfo, SwapChainCommandBuffer* _output)
{
	CommandBufferBase::InitializeCommandBuffer(_createInfo, _output);
	_output->InitializeSyncObjects();
}

void SwapChainCommandBuffer::InitializeSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}

void SwapChainCommandBuffer::RecordCommandBuffer(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass->GetRenderPass();
	renderPassInfo.framebuffer = frameBuffer->GetFrameBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetGraphicsPipeline());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void SwapChainCommandBuffer::Cleanup()
{
	std::cout << "[Cleaning] SwapChainCommandBuffer" << std::endl;

	vkDestroySemaphore(logicalDevice, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice, renderFinishedSemaphore, nullptr);
	vkDestroyFence(logicalDevice, inFlightFence, nullptr);

	std::cout << "[Cleaned] SwapChainCommandBuffer" << std::endl;
}

const VkSemaphore& SwapChainCommandBuffer::GetImageAvailableSemaphore() const
{
	return imageAvailableSemaphore;
}

const VkSemaphore& SwapChainCommandBuffer::GetRenderFinishedSemaphore() const
{
	return renderFinishedSemaphore;
}

const VkFence& SwapChainCommandBuffer::GetInFlightFence() const
{
	return inFlightFence;
}
