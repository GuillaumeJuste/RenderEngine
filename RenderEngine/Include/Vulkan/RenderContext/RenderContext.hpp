#pragma once

#ifndef RENDERENGINE_RENDERCONTEXT
#define RENDERENGINE_RENDERCONTEXT

#include "Vulkan/RenderContext/RenderContextCreateInfo.hpp"
#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/SwapChain/SwapChain.hpp"
#include "Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Vulkan/CommandPool/CommandPool.hpp"
#include "Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBuffer.hpp"
#include "Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to handle a pipeline to render in a window
	*/
	class RenderContext
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
		void CreateGraphicsPipeline();
		
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
		void CreateCommandBuffer();

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
		static void InitalizeRenderContext(const RenderContextCreateInfo& _createInfo, RenderContext* _output);

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