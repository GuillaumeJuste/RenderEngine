#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTCREATEINFO
#define RENDERENGINE_RENDERCONTEXTCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Vulkan
{
	struct RenderContextCreateInfo
	{
		VkInstance instance;
		Surface* surface;
		Window* window;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice;
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		RenderContextCreateInfo() = default;
	};
}

#endif