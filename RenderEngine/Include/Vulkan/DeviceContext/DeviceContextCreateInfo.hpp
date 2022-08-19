#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTCREATEINFO
#define RENDERENGINE_DEVICECONTEXTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"

namespace RenderEngine::Vulkan
{
	struct DeviceContextCreateInfo
	{
		VkInstance instance = VK_NULL_HANDLE;
		WindowProperties* windowProperties;

		DeviceContextCreateInfo() = default;
	};
}

#endif