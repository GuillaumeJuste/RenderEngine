#pragma once

#ifndef RENDERENGINE_DEVICECREATEINFO
#define RENDERENGINE_DEVICECREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	struct DeviceContextCreateInfo
	{
		VkInstance instance = VK_NULL_HANDLE;
		Surface* surface;
		Window* window;

		DeviceContextCreateInfo() = default;
		DeviceContextCreateInfo(const VkInstance& _instance, Surface* _surface, Window* _window);
	};
}

#endif