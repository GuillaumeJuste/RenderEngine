#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include "Vulkan/DeviceContext/DeviceContextCreateInfo.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Vulkan/SwapChain/SwapChainSupportDetails.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/RenderContext/RenderContext.hpp"

namespace RenderEngine::Vulkan
{
	class DeviceContext
	{
	private:
		VkInstance instance;
		Window* window;

		Surface surface;

		QueueFamilyIndices queueFamilyIndices;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice;
		
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		RenderContext renderContext;

		void CreateSurface();
		bool IsDeviceSuitable(const VkPhysicalDevice& _device);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void PickPhysicalDevice();
		void CreateLogicalDevice();

	public:
		DeviceContext() = default;
		~DeviceContext() = default;

		static void InitalizeDevice(const DeviceContextCreateInfo& _createInfo, DeviceContext* _output);

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		RenderContext* GetRenderContext();

		void Cleanup();
	};
}

#endif