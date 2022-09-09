#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTVKCREATEINFO
#define RENDERENGINE_RENDERCONTEXTVKCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for RenderContext Initialization
	*/
	struct RenderContextVkCreateInfo
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
		RenderContextVkCreateInfo() = default;
	};
}

#endif