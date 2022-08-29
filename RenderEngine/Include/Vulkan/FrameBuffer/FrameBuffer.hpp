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

		/// logical device
		VkDevice logicalDevice;
		
		/// renderPass
		RenderPass* renderPass;
		
		/// Image View
		const ImageView* imageView;
		
		/// number of image in the swapchain
		size_t swapChainImageCount;
		
		/// swapchaine xtent
		VkExtent2D swapChainExtent;

		///Framebuffers
		std::vector<VkFramebuffer> framebuffers;

	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		/**
		 * @brief Initiliaze frame buffer
		 * @param _frameBufferCreateInfo framebuffer creation info
		 * @param _output framebuffer to initiliaze
		*/
		static void InitializeFrameBuffer(FrameBufferCreateInfo _frameBufferCreateInfo, FrameBuffer* _output);
		
		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		/// Get framebuffers vector
		const std::vector<VkFramebuffer>& GetFrameBuffers() const;
	};

}

#endif