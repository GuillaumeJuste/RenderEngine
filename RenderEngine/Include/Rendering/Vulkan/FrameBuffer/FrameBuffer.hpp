#pragma once

#ifndef RENDERENGINE_FRAMEBUFFER
#define RENDERENGINE_FRAMEBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/FrameBuffer/FrameBufferVkCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan FrameBuffer
	*/
	class FrameBuffer
	{
	private:

		/// logical device
		VkDevice logicalDevice = nullptr;
		
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