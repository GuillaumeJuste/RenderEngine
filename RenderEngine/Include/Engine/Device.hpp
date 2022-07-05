#pragma once

#ifndef RENDERENGINE_DEVICE
#define RENDERENGINE_DEVICE

#include "Vulkan/VulkanBaseInclude.hpp"

#include "Engine/Surface.hpp"
#include "SwapChain/SwapChain.hpp"

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

		bool IsDeviceSuitable(const VkPhysicalDevice& _device);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		

		void PickPhysicalDevice();
		void CreateLogicalDevice();

		void CreateSwapChain();

	public:
		Device() = default;
		~Device() = default;

		void InitalizeDevice(const VkInstance& _instance, const Surface& _surface, Window* _window);

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		void Cleanup();
	};
}

#endif