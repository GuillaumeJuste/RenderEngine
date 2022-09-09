#pragma once

#ifndef RENDERENGINE_FRAMEBUFFER
#define RENDERENGINE_FRAMEBUFFER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/FrameBuffer/FrameBufferVkCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan FrameBuffer
	*/
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
		
		/// swapchaine extent
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
		static void InitializeFrameBuffer(const FrameBufferVkCreateInfo& _frameBufferCreateInfo, FrameBuffer* _output);
		
		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		/// Get framebuffers vector
		const std::vector<VkFramebuffer>& GetFrameBuffers() const;
	};

}

#endif