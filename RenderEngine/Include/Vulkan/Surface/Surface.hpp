#pragma once

#ifndef RENDERENGINE_SURFACE
#define RENDERENGINE_SURFACE

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
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