#pragma once

#ifndef RENDERENGINE_DEPTHBUFFERVKCREATEINFO
#define RENDERENGINE_DEPTHBUFFERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Rendering
{
	struct DepthBufferVkCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		VkExtent2D swapChainExtent;

		VkQueue graphicsQueue;
		CommandPool* commandPool;

		/// default constructor
		DepthBufferVkCreateInfo() = default;
	};
}

#endif