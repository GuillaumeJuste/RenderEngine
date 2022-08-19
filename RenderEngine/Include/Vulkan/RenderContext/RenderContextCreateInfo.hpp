#pragma once

#ifndef RENDERENGINE_RENDERCONTEXTCREATEINFO
#define RENDERENGINE_RENDERCONTEXTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Vulkan
{
	struct RenderContextCreateInfo
	{
		VkInstance instance;
		Window* window;

		RenderContextCreateInfo() = default;
	};
}

#endif