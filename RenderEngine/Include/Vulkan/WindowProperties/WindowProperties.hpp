#pragma once

#ifndef RENDERENGINE_WINDOWPROPERTIES
#define RENDERENGINE_WINDOWPROPERTIES

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/Surface/Surface.hpp"


namespace RenderEngine::Vulkan
{
	struct WindowProperties
	{
		Window* window;
		Surface surface;

		WindowProperties() = default;
	};
}

#endif