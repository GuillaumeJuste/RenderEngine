#pragma once

#ifndef RENDERENGINE_WINDOWPROPERTIES
#define RENDERENGINE_WINDOWPROPERTIES

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Surface/Surface.hpp"
#include "Window/Glfw/Window.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Rendering
{
	/**
	 * @brief struct holding a pair of window and vulkan surface representing it
	*/
	struct WindowProperties
	{
		/// window
		GLFW::Window* window;

		/// surface
		Surface surface;

		/// default constructor
		WindowProperties() = default;
	};
}

#endif