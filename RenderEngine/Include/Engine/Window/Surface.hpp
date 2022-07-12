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
		VkInstance instance = VK_NULL_HANDLE;
		Window* window = nullptr;

		VkSurfaceKHR vkSurface = VK_NULL_HANDLE;

	public:
		Surface() = default;
		~Surface() = default;

		static void InitializeSurface(const VkInstance& _instance, Window* _window, Surface* _output);

		void Cleanup();
		const VkSurfaceKHR& GetVkSurface() const;
	};
}

#endif