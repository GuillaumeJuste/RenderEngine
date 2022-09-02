#pragma once

#ifndef RENDERENGINE_SWAPCHAINCREATEINFO
#define RENDERENGINE_SWAPCHAINCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for SwapChain Initialization
	*/
	struct SwapChainCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// window properties
		WindowProperties* windowProperties;
		
		/// Queue family indices
		QueueFamilyIndices queueFamilyIndices;

		SwapChainCreateInfo() = default;
	};
}

#endif