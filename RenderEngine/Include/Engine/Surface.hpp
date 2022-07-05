#pragma once

#ifndef RENDERENGINE_SURFACE
#define RENDERENGINE_SURFACE

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{

	class Window;

	class Surface
	{
	private:
		VkInstance instance;
		Window* window;

		VkSurfaceKHR vkSurface;

	public:
		Surface() = default;
		~Surface() = default;

		void InitializeSurface(const VkInstance& _instance, Window* _window);

		void Cleanup();
		const VkSurfaceKHR& GetVkSurface() const;
	};
}

#endif