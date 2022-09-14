#pragma once

#ifndef RENDERENGINE_RENDERCONTEXT
#define RENDERENGINE_RENDERCONTEXT

#include "Engine/Base/Interface/IRenderContext.hpp"
using namespace RenderEngine::Engine::Base;

#include "Engine/Vulkan/RenderContext/RenderContextVkCreateInfo.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Surface/Surface.hpp"
#include "Engine/Vulkan/SwapChain/SwapChain.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "Engine/Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBuffer.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to handle a pipeline to render in a window
	*/
	class RenderContext : public IRenderContext
	{
	private:
		///instance
		VkInstance instance;

		/// window properties
		WindowProperties* windowProperties;
		
		/// frame buffer was resized
		bool frameBufferWasResized = false;

		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		
		/// logical device
		VkDevice logicalDevice;
		
		/// queue family indices
		QueueFamilyIndices queueFamilyIndices;
		
		/// graphics queue
		VkQueue graphicsQueue;
		
		/// present queue
		VkQueue presentQueue;

		/// swap chain
		SwapChain swapChain;
		
		/// render pass
		RenderPass renderPass;
		
		/// graphics pipeline
		GraphicsPipeline graphicsPipeline;
		
		/// framebuffer
		FrameBuffer frameBuffer;
		
		/// command pool
		CommandPool commandPool;
		
		///command buffers
		std::vector<SwapChainCommandBuffer> commandBuffers;

		/// current frame
		uint32_t currentFrame = 0;

		/// tmp variables
		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;
		BufferObject stagingBufferObject;

		/**
		 * @brief Create swapchain
		*/
		void CreateSwapChain();

		/**
		 * @brief Create renderpass
		*/
		void CreateRenderPass();

		/**
		 * @brief Create Graphics Pipeline
		*/
		void CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& _createInfo);
		
		/**
		 * @brief Create Framebuffer
		*/
		void CreateFrameBuffer();

		/**
		 * @brief Create Command Pool
		*/
		void CreateCommandPool();

		/**
		 * @brief Create Command Buffer
		*/
		void CreateCommandBuffer(const SwapChainCommandBufferCreateInfo& _createInfo);

		/// TMP classes
		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

		/**
		 * @brief Clean swapchain
		*/
		void CleanUpSwapChain();

		/**
		 * @brief recreate swapchain
		*/
		void RecreateSwapChain();

		/**
		 * @brief Callback on window resized
		*/
		void FrameBufferResizedCallback();

	public:
		/// default constructor
		RenderContext() = default;

		/// default destructor
		~RenderContext() = default;

		/**
		 * @brief Initilize render context
		 * @param _createInfo 
		 * @param _output 
		*/
		static void InitalizeRenderContext(const RenderContextVkCreateInfo& _createInfo, RenderContext* _output);

		/**
		 * @brief Draw current frame
		*/
		void DrawFrame();

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();
	};
}

#endif