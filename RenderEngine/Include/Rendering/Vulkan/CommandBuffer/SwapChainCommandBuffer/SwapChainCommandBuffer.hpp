#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFER
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Rendering/Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBufferVkCreateInfo.hpp"
#include "Rendering/Vulkan/Scene/VkScene.hpp"


namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of swapchain command buffer
	*/
	class SwapChainCommandBuffer : public CommandBuffer
	{
	private:
		/**
		 * @brief render pass
		*/
		RenderPass* renderPass;

		SwapChainCommandBufferCreateInfo commandBufferCreateInfo;

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
		 * @brief record command buffer to render a scene
		 * @param _imageIndex index of the frame to render
		 * @param _scene scene to render
		*/
		void RecordCommandBuffer(uint32_t _imageIndex, int _currentFrame, VkScene* _scene);


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