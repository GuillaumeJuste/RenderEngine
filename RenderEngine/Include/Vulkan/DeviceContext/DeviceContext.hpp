#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include "Vulkan/DeviceContext/DeviceContextCreateInfo.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Vulkan/SwapChain/SwapChainSupportDetails.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/RenderContext/RenderContext.hpp"
#include "Vulkan/DeviceContext/PhysicalDeviceProperties.hpp"

namespace RenderEngine::Vulkan
{
	class DeviceContext
	{
	private:
		VkInstance instance;

		WindowProperties* windowProperties;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		PhysicalDeviceProperties physicalDeviceProperties;

		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue presentQueue = VK_NULL_HANDLE;

		std::vector<RenderContext> renderContexts;

		bool IsDeviceSuitable(PhysicalDeviceProperties* _properties);
		bool FindQueueFamilies(VkPhysicalDevice _device, QueueFamilyIndices* _output);
		
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		
		void PickPhysicalDevice();
		PhysicalDeviceProperties UserSelectPhysicalDevice(std::vector<PhysicalDeviceProperties> _physicalDevicesProperties);
		void CreateLogicalDevice();

	public:
		DeviceContext() = default;
		~DeviceContext() = default;

		static void InitalizeDevice(const DeviceContextCreateInfo& _createInfo, DeviceContext* _output);

		RenderContext* AddRenderContext(RenderContextCreateInfo _renderContextCreateInfo);

		const VkPhysicalDevice& GetPhysicalDevice() const;
		const uint32_t& GetGraphicsQueueIndex() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;

		void Cleanup();
	};
}

#endif