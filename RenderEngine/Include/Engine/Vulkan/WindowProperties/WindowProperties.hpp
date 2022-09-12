#pragma once

#ifndef RENDERENGINE_WINDOWPROPERTIES
#define RENDERENGINE_WINDOWPROPERTIES

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Surface/Surface.hpp"
#include "Window/Glfw/Window.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Engine::Vulkan
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