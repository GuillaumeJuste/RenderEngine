#pragma once

#ifndef RENDERENGINE_RENDERCONTEXT
#define RENDERENGINE_RENDERCONTEXT

#include "Rendering/Base/Interface/IRenderContext.hpp"

#include "Rendering/Vulkan/RenderContext/RenderContextVkCreateInfo.hpp"
#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Surface/Surface.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"
#include "Rendering/Vulkan/CommandBuffer/SwapChainCommandBuffer/SwapChainCommandBuffer.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/Scene/Data/SceneData.hpp"
#include "Rendering/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Rendering/Vulkan/DepthBuffer/DepthBuffer.hpp"


namespace RenderEngine::Rendering
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

		/// command pool
		CommandPool* commandPool;

		/// swap chain
		SwapChain swapChain;
		
		/// render pass
		RenderPass renderPass;
		
		/// framebuffer
		FrameBuffer frameBuffer;
		
		DepthBuffer depthBuffer;
		
		///command buffers
		std::vector<SwapChainCommandBuffer> commandBuffers;

		std::forward_list<SceneData> scenesData;

		/// current frame
		uint32_t currentFrame = 0;

		/**
		 * @brief Create swapchain
		*/
		void CreateSwapChain();

		/**
		 * @brief Create renderpass
		*/
		void CreateRenderPass();

		/**
		 * @brief Create Framebuffer
		*/
		void CreateFrameBuffer();

		void CreateDepthBuffer();

		/**
		 * @brief Create Command Buffer
		*/
		void CreateCommandBuffer(const SwapChainCommandBufferCreateInfo& _createInfo);

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

		VkScene* WasSceneLoaded(RenderEngine::SceneGraph::Scene* _scene);

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

		void DrawScene(RenderEngine::SceneGraph::Scene* _scene);

		VkScene* LoadScene(RenderEngine::SceneGraph::Scene* _scene);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();
	};
}

#endif