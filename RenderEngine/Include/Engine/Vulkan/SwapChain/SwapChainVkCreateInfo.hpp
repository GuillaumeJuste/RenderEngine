#pragma once

#ifndef RENDERENGINE_SWAPCHAINVKCREATEINFO
#define RENDERENGINE_SWAPCHAINVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/WindowProperties/WindowProperties.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/DeviceContext/QueueFamilyIndices.hpp"

namespace RenderEngine::Engine::Vulkan
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