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
	class RenderContext
	{
	private:
		VkInstance instance;

		WindowProperties* windowProperties;
		bool frameBufferWasResized = false;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice;
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		SwapChain swapChain;
		RenderPass renderPass;
		GraphicsPipeline graphicsPipeline;
		FrameBuffer frameBuffer;
		CommandPool commandPool;
		std::vector<SwapChainCommandBuffer> commandBuffers;

		uint32_t currentFrame = 0;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;
		BufferObject stagingBufferObject;

		void CreateSwapChain();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFrameBuffer();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

		void CleanUpSwapChain();
		void RecreateSwapChain();

		void FrameBufferResizedCallback();

	public:
		RenderContext() = default;
		~RenderContext() = default;

		static void InitalizeRenderContext(const RenderContextCreateInfo& _createInfo, RenderContext* _output);

		void DrawFrame();

		void Cleanup();
	};
}

#endif