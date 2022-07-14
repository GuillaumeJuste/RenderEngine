#pragma once

#ifndef RENDERENGINE_FRAMEBUFFER
#define RENDERENGINE_FRAMEBUFFER

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/FrameBuffer/FrameBufferCreateInfo.hpp"

namespace RenderEngine
{

	class FrameBuffer
	{
	private:
		VkDevice* logicalDevice;
		RenderPass* renderPass;
		const ImageView* imageView;
		size_t swapChainImageCount;
		VkExtent2D swapChainExtent;

		std::vector<VkFramebuffer> swapChainFramebuffers;

	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		static void InitializeFrameBuffer(FrameBufferCreateInfo _frameBufferCreateInfo, FrameBuffer* _output);
		void Cleanup();
	};

}

#endif