#pragma once

#ifndef RENDERENGINE_VKRESOURCEMANAGERCREATEINFO
#define RENDERENGINE_VKRESOURCEMANAGERCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for RenderContext Initialization
	*/
	struct VkResourceManagerCreateInfo
	{
		/// Physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		/// graphics queue
		VkQueue graphicsQueue;

		CommandPool* commandPool;

		/// default constructor
		VkResourceManagerCreateInfo() = default;
	};
}

#endif