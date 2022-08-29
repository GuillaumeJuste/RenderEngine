#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTCREATEINFO
#define RENDERENGINE_RENDERCONTEXTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Vulkan
{
	struct RenderContextCreateInfo
	{
		/// vulkan instance
		VkInstance instance;

		/// window properties
		WindowProperties* windowProperties;

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

		/// default constructor
		RenderContextCreateInfo() = default;
	};
}

#endif