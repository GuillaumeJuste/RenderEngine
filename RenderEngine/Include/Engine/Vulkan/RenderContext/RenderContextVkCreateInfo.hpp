#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTVKCREATEINFO
#define RENDERENGINE_RENDERCONTEXTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Surface/Surface.hpp"
#include "Engine/Vulkan/WindowProperties/WindowProperties.hpp"
#include "Engine/Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Engine::Vulkan
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