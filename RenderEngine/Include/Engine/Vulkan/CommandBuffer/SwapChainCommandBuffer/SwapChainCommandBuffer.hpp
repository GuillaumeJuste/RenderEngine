#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFER
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFER

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandBuffer/Base/CommandBufferBase.hpp"
#include "Engine/Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBufferVkCreateInfo.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of swapchain command buffer
	*/
	class SwapChainCommandBuffer : public CommandBufferBase
	{
	private:
		/**
		 * @brief render pass
		*/
		RenderPass* renderPass;

		SwapChainCommandBufferCreateInfo commandBufferCreateInfo;

		/**
		 * @brief graphic pipline to use for render
		*/
		GraphicsPipeline* graphicsPipeline;

		/**
		 * @brief framebuffer
		*/
		FrameBuffer* frameBuffer;

		/// Swapchain
		SwapChain* swapChain;

		/// Command buffer image available semaphore
		VkSemaphore imageAvailableSemaphore;

		/// Command buffer render finished semaphore
		VkSemaphore renderFinishedSemaphore;
		
		/// Command buffer in flight fence
		VkFence inFlightFence;
		
		VkViewport viewport{};

		VkRect2D scissor{};

		/**
		 * @brief tmp variables
		*/
		BufferObject* vertexBufferObject;
		BufferObject* indexBufferObject;

		/**
		 * @brief initialize semaphores and fence 
		*/
		void InitializeSyncObjects();

		void SwapChainExtentResizedCallback();
	public:
		/// default constructor
		SwapChainCommandBuffer() = default;

		/// default destructor
		~SwapChainCommandBuffer() = default;

		/**
		 * @brief Initialize Command buffer
		 * @param _createInfo buffer creation info
		 * @param _output command buffer to initialize 
		*/
		static void InitializeCommandBuffer(const SwapChainCommandBufferVkCreateInfo& _createInfo, SwapChainCommandBuffer* _output);

		/**
		 * @brief record command buffer to render a frame
		 * @param imageIndex index of the frame to render
		*/
		void RecordCommandBuffer(uint32_t imageIndex);

		/**
		 * @brief clean vulkan classes
		*/
		void Cleanup();

		/// Get imageAvailableSemaphore
		const VkSemaphore& GetImageAvailableSemaphore() const;

		/// Get renderFinishedSemaphore
		const VkSemaphore& GetRenderFinishedSemaphore() const;
		
		/// Get inFlightFence
		const VkFence& GetInFlightFence() const;
	};
}

#endif