#pragma once

#ifndef RENDERENGINE_WINDOWPROPERTIES
#define RENDERENGINE_WINDOWPROPERTIES

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/Surface/Surface.hpp"


namespace RenderEngine::Vulkan
{
	/**
	 * @brief struct holding a pair of window and vulkan surface representing it
	*/
	struct WindowProperties
	{
		/// window
		Window* window;

		/// surface
		Surface surface;

		/// default constructor
		WindowProperties() = default;
	};
}

#endif