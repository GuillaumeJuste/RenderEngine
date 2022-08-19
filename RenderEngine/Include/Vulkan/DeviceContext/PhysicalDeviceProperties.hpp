#pragma once

#ifndef RENDERENGINE_PHYSICALDEVICEPROPERTIES
#define RENDERENGINE_PHYSICALDEVICEPROPERTIES

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"

namespace RenderEngine::Vulkan
{
	struct PhysicalDeviceProperties
	{
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		QueueFamilyIndices queueFamilyIndices;
		VkPhysicalDeviceProperties properties;

		PhysicalDeviceProperties() = default;
		PhysicalDeviceProperties(VkPhysicalDevice _physicalDevice);
	};
}

#endif