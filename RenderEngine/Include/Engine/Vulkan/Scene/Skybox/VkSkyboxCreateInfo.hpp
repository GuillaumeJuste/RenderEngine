#pragma once

#ifndef RENDERENGINE_VKSKYBOXCREATEINFO
#define RENDERENGINE_VKSKYBOXCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/SwapChain/SwapChain.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for VkSkybox Initialization
	*/
	struct VkSkyboxCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		SwapChain* swapchain;

		RenderPass* renderpass;

		/// default constructor
		VkSkyboxCreateInfo() = default;
	};
}

#endif