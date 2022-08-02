#pragma once

#ifndef RENDERENGINE_RENDERCONTEXT
#define RENDERENGINE_RENDERCONTEXT

#include "Engine/RenderContext/RenderContextCreateInfo.hpp"
#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/SwapChain/SwapChain.hpp"
#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/FrameBuffer/FrameBuffer.hpp"
#include "Engine/CommandPool/CommandPool.hpp"
#include "Engine/CommandBuffer/SwapChainCommandBuffer.hpp"
#include "Engine/BufferObject/BufferObject.hpp"

namespace RenderEngine::Vulkan
{
	class RenderContext
	{
	private:
		VkInstance instance;
		Surface* surface;
		Window* window;
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

	public:
		RenderContext() = default;
		~RenderContext() = default;

		static void InitalizeRenderContext(const RenderContextCreateInfo& _createInfo, RenderContext* _output);

		void DrawFrame();

		void Cleanup();
	};
}

#endif