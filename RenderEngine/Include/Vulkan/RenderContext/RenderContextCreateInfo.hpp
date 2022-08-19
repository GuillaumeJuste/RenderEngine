#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTCREATEINFO
#define RENDERENGINE_RENDERCONTEXTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Vulkan
{
	struct RenderContextCreateInfo
	{
		VkInstance instance;
		WindowProperties* windowProperties;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice;
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		RenderContextCreateInfo() = default;
	};
}

#endif