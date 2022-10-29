#include "Engine/Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBuffer.hpp"

#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

#include <iostream>
#include <array>

using namespace RenderEngine::Engine::Vulkan;

void SwapChainCommandBuffer::InitializeCommandBuffer(const SwapChainCommandBufferVkCreateInfo& _createInfo, SwapChainCommandBuffer* _output)
{
	_output->renderPass = _createInfo.renderPass;
	_output->frameBuffer = _createInfo.frameBuffer;
	_output->commandBufferCreateInfo = _createInfo.commandBufferCreateInfo;
	_output->swapChain = _createInfo.swapChain;
	
	CommandBuffer::InitializeCommandBuffer(_createInfo, _output);
	_output->SwapChainExtentResizedCallback();
	_output->swapChain->SwapchainExtentResized.Add(_output, &SwapChainCommandBuffer::SwapChainExtentResizedCallback);

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
		vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}


void SwapChainCommandBuffer::RecordCommandBuffer(uint32_t _imageIndex, int _currentFrame, VkScene* _scene)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkExtent2D swapChainExtent = swapChain->GetExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass->GetRenderPass();
	renderPassInfo.framebuffer = frameBuffer->GetFrameBuffers()[_imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	_scene->Draw(commandBuffer, _currentFrame);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void SwapChainCommandBuffer::SwapChainExtentResizedCallback()
{
	VkExtent2D swapChainExtent = swapChain->GetExtent(); 
	
	if (commandBufferCreateInfo.customViewport == false)
	{
		viewport.width = static_cast<float>(swapChainExtent.width);
		viewport.height = -static_cast<float>(swapChainExtent.height); 
		viewport.x = 0.0f;
		viewport.y = static_cast<float>(swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
	}
	else
	{
		viewport.x = commandBufferCreateInfo.viewportData.X;
		viewport.y = commandBufferCreateInfo.viewportData.Y;
		viewport.width = commandBufferCreateInfo.viewportData.width;
		viewport.height = commandBufferCreateInfo.viewportData.height;
		viewport.minDepth = commandBufferCreateInfo.viewportData.minDepth;
		viewport.maxDepth = commandBufferCreateInfo.viewportData.maxDepth;
	}
	
	if (commandBufferCreateInfo.customScissor == false)
	{
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;
	}
	else
	{
		scissor.offset = { (int)commandBufferCreateInfo.scissorData.offset.X, (int)commandBufferCreateInfo.scissorData.offset.Y };
		scissor.extent = { (unsigned int)commandBufferCreateInfo.scissorData.extent.X, (unsigned int)commandBufferCreateInfo.scissorData.extent.Y };
	}
}

void SwapChainCommandBuffer::Cleanup()
{
	swapChain->SwapchainExtentResized.Remove(this, &SwapChainCommandBuffer::SwapChainExtentResizedCallback);
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
