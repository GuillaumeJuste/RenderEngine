#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTCREATEINFO
#define RENDERENGINE_DEVICECONTEXTCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	struct DeviceContextCreateInfo
	{
		VkInstance instance = VK_NULL_HANDLE;
		Window* window;

		DeviceContextCreateInfo() = default;
	};
}

#endif