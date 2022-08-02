#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include "Engine/DeviceContext/DeviceContextCreateInfo.hpp"
#include "Engine/DeviceContext/QueueFamiliyIndices.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/SwapChain/SwapChain.hpp"
#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/FrameBuffer/FrameBuffer.hpp"
#include "Engine/CommandPool/CommandPool.hpp"
#include "Engine/CommandBuffer/SwapChainCommandBuffer.hpp"
#include "Engine/BufferObject/BufferObject.hpp"
#include "Engine/SwapChain/SwapChainSupportDetails.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	class DeviceContext
	{
	private:
		VkInstance instance;
		Surface* surface;
		Window* window;

		QueueFamilyIndices queueFamilyIndices;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice logicalDevice;
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

		bool IsDeviceSuitable(const VkPhysicalDevice& _device);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void PickPhysicalDevice();
		void CreateLogicalDevice();

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
		DeviceContext() = default;
		~DeviceContext() = default;

		static void InitalizeDevice(const DeviceContextCreateInfo& _createInfo, DeviceContext* _output);

		void DrawFrame();

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		void Cleanup();
	};
}

#endif