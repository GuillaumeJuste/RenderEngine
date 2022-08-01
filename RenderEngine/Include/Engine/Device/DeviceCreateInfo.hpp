#pragma once

#ifndef RENDERENGINE_DEVICECREATEINFO
#define RENDERENGINE_DEVICECREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Surface.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	struct DeviceCreateInfo
	{
		VkInstance instance = VK_NULL_HANDLE;
		Surface* surface;
		Window* window;

		DeviceCreateInfo() = default;
		DeviceCreateInfo(const VkInstance& _instance, Surface* _surface, Window* _window);
	};
}

#endif