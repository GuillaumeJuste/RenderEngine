#include "Vulkan/FrameBuffer/FrameBuffer.hpp"

#include <iostream>

using namespace RenderEngine::Vulkan;

void FrameBuffer::InitializeFrameBuffer(const FrameBufferCreateInfo& _frameBufferCreateInfo, FrameBuffer* _output)
{
	_output->logicalDevice = _frameBufferCreateInfo.logicalDevice;
	_output->renderPass = _frameBufferCreateInfo.renderPass;
	_output->imageView = _frameBufferCreateInfo.imageView;
	_output->swapChainImageCount = _frameBufferCreateInfo.swapChainImageCount;
	_output->swapChainExtent = _frameBufferCreateInfo.swapChainExtent;

	_output->framebuffers.resize(_frameBufferCreateInfo.swapChainImageCount);

	for (size_t i = 0; i < _frameBufferCreateInfo.swapChainImageCount; i++) {
		VkImageView attachments[] = {
			_frameBufferCreateInfo.imageView->GetSwapChainImageViews()[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _frameBufferCreateInfo.renderPass->GetRenderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _frameBufferCreateInfo.swapChainExtent.width;
		framebufferInfo.height = _frameBufferCreateInfo.swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_frameBufferCreateInfo.logicalDevice, &framebufferInfo, nullptr, &_output->framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void FrameBuffer::Cleanup()
{
	for (auto framebuffer : framebuffers) 
	{
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}
	std::cout << "[Cleaned] Frame Buffer" << std::endl;
}

const std::vector<VkFramebuffer>& FrameBuffer::GetFrameBuffers() const
{
	return framebuffers;
}