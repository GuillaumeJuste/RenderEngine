#include "Engine/Surface.hpp"

#include <stdexcept>

#include "Engine/Window.hpp"

using namespace RenderEngine;

void Surface::InitializeSurface(const VkInstance& _instance, Window* _window)
{
	instance = _instance;
	window = _window;

	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window->GetGLFWWindow());
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &vkSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void Surface::Cleanup()
{
	vkDestroySurfaceKHR(instance, vkSurface, nullptr);
}

const VkSurfaceKHR& Surface::GetVkSurface() const
{
	return vkSurface;
}