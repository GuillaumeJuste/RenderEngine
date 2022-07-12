#pragma once

#ifndef RENDERENGINE_DEVICECREATEINFO
#define RENDERENGINE_DEVICECREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Surface.hpp"

namespace RenderEngine
{
	class Window;

	struct DeviceCreateInfo
	{
		VkInstance instance = VK_NULL_HANDLE;
		Surface surface;
		Window* window;

		DeviceCreateInfo() = default;
		DeviceCreateInfo(const VkInstance& _instance, const Surface& _surface, Window* _window);
	};
}

#endif