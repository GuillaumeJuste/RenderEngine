#pragma once

#ifndef RENDERENGINE_VKSKYBOXCREATEINFO
#define RENDERENGINE_VKSKYBOXCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for VkSkybox Initialization
	*/
	struct VkSkyboxCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		/// logical device
		VkDevice logicalDevice;

		SwapChain* swapchain;

		RenderPass* renderpass;

		/// default constructor
		VkSkyboxCreateInfo() = default;
	};
}

#endif