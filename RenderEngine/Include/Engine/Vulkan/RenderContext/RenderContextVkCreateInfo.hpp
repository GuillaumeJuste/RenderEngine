#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTVKCREATEINFO
#define RENDERENGINE_RENDERCONTEXTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Surface/Surface.hpp"
#include "Engine/Vulkan/WindowProperties/WindowProperties.hpp"
#include "Engine/Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Engine/Base/CreateInfo/IRenderContextCreateInfo.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

using namespace RenderEngine::Engine::Base;

namespace RenderEngine::Engine::Vulkan
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