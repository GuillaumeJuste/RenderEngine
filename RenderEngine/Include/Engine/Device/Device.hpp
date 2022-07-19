#pragma once

#ifndef RENDERENGINE_DEVICE
#define RENDERENGINE_DEVICE

#include "Engine/Device/QueueFamiliyIndices.hpp"
#include "Engine/Window/Surface.hpp"
#include "Engine/SwapChain/SwapChain.hpp"
#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Device/DeviceCreateInfo.hpp"
#include "Engine/FrameBuffer/FrameBuffer.hpp"
#include "Engine/CommandPool/CommandPool.hpp"
#include "Engine/CommandBuffer/CommandBuffer.hpp"
#include "Engine/SyncObjects/SyncObjects.hpp"

namespace RenderEngine
{
	struct SwapChainSupportDetails;
	class Window;

	class Device
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

		CommandBuffer commandBuffer;

		SyncObjects syncObjects;

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
		void CreateSyncObjects();

	public:
		Device() = default;
		~Device() = default;

		static void InitalizeDevice(const DeviceCreateInfo& _createInfo, Device* _output);

		void DrawFrame();

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		void Cleanup();
	};
}

#endif