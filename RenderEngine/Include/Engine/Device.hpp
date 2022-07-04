#pragma once

#ifndef RENDERENGINE_DEVICE
#define RENDERENGINE_DEVICE

#include <GLFW/glfw3.h>

namespace RenderEngine
{
	class Device
	{
	private:
		VkInstance instance;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		uint32_t graphicsQueueIndex;

		VkDevice logicalDevice;
		VkQueue graphicsQueue;

		bool IsDeviceSuitable(VkPhysicalDevice _device);

		void PickPhysicalDevice();
		void CreateLogicalDevice();

	public:
		Device() = default;
		Device(VkInstance _instance);
		~Device() = default;

		const VkPhysicalDevice& GetPhysicalDevice();
		const uint32_t& GetGraphicsQueueIndex();
		const VkDevice& GetLogicalDevice();
		const VkQueue& GetGraphicsQueue();

		void Cleanup();
	};
}

#endif