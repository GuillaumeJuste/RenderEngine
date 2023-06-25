#include "Rendering/Vulkan/FrameBuffer/FrameBuffer.hpp"

#include <iostream>
#include <vector>

using namespace RenderEngine::Rendering;

void FrameBuffer::InitializeFrameBuffer(const FrameBufferVkCreateInfo& _frameBufferCreateInfo, FrameBuffer* _output)
{
	_output->logicalDevice = _frameBufferCreateInfo.logicalDevice;

	_output->framebuffers.resize(_frameBufferCreateInfo.swapChainImageCount);

	for (size_t i = 0; i < _frameBufferCreateInfo.swapChainImageCount; i++) 
	{
		std::vector<VkImageView> attachments;
		
		if (_frameBufferCreateInfo.colorImage != nullptr)
			attachments.push_back(_frameBufferCreateInfo.colorImage->GetImageView());

		if (_frameBufferCreateInfo.depthBuffer != nullptr)
			attachments.push_back(_frameBufferCreateInfo.depthBuffer->GetVkImageView());

		attachments.push_back(_frameBufferCreateInfo.imageViews[i]);

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _frameBufferCreateInfo.renderPass->GetRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
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
}

const std::vector<VkFramebuffer>& FrameBuffer::GetFrameBuffers() const
{
	return framebuffers;
}