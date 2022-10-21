#pragma once

#ifndef RENDERENGINE_DEPTHBUFFERVKCREATEINFO
#define RENDERENGINE_DEPTHBUFFERVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

namespace RenderEngine::Engine::Vulkan
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