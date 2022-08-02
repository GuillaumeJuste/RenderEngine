#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTCREATEINFO
#define RENDERENGINE_DEVICECONTEXTCREATEINFO

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