#pragma once

#ifndef RENDERENGINE_SWAPCHAINVKCREATEINFO
#define RENDERENGINE_SWAPCHAINVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/WindowProperties/WindowProperties.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/DeviceContext/QueueFamilyIndices.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for SwapChain Initialization
	*/
	struct SwapChainVkCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// window properties
		WindowProperties* windowProperties;
		
		/// Queue family indices
		QueueFamilyIndices queueFamilyIndices;

		SwapChainVkCreateInfo() = default;
	};
}

#endif