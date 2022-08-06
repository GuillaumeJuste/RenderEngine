#pragma once

#ifndef RENDERENGINE_FRAMEBUFFER
#define RENDERENGINE_FRAMEBUFFER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/FrameBuffer/FrameBufferCreateInfo.hpp"

namespace RenderEngine::Vulkan
{

	class FrameBuffer
	{
	private:
		VkDevice logicalDevice;
		RenderPass* renderPass;
		const ImageView* imageView;
		size_t swapChainImageCount;
		VkExtent2D swapChainExtent;

		std::vector<VkFramebuffer> framebuffers;

	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		static void InitializeFrameBuffer(FrameBufferCreateInfo _frameBufferCreateInfo, FrameBuffer* _output);
		void Cleanup();

		const std::vector<VkFramebuffer>& GetFrameBuffers() const;
	};

}

#endif