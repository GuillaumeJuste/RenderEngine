#pragma once

#ifndef RENDERENGINE_DEVICE
#define RENDERENGINE_DEVICE


#include "Engine/Window/Surface.hpp"
#include "Engine/SwapChain/SwapChain.hpp"
#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Device/DeviceCreateInfo.hpp"

namespace RenderEngine
{
	struct SwapChainSupportDetails;
	class Window;

	class Device
	{
	private:
		VkInstance instance;
		Surface surface;
		Window* window;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		uint32_t graphicsQueueIndex;

		VkDevice logicalDevice;
		VkQueue graphicsQueue;

		VkQueue presentQueue;

		SwapChain swapChain;

		GraphicsPipeline graphicsPipeline;

		bool IsDeviceSuitable(const VkPhysicalDevice& _device);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		

		void PickPhysicalDevice();
		void CreateLogicalDevice();

		void CreateSwapChain();

		void CreateGraphicsPipeline();

	public:
		Device() = default;
		~Device() = default;

		static void InitalizeDevice(const DeviceCreateInfo& _createInfo, Device* _output);

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		void Cleanup();
	};
}

#endif