#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTVKCREATEINFO
#define RENDERENGINE_RENDERCONTEXTVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Surface/Surface.hpp"
#include "Rendering/Vulkan/WindowProperties/WindowProperties.hpp"
#include "Rendering/Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Rendering/Base/CreateInfo/IRenderContextCreateInfo.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for RenderContext Initialization
	*/
	struct RenderContextVkCreateInfo
	{
		/// vulkan instance
		VkInstance instance;

		IRenderContextCreateInfo renderContextCreateInfo;

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

		CommandPool* commandPool;

		/// default constructor
		RenderContextVkCreateInfo() = default;
	};
}

#endif