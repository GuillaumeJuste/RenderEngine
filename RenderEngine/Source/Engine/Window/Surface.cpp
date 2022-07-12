#include "Engine/Window/Surface.hpp"

#include <stdexcept>

#include "Engine/Window/Window.hpp"

using namespace RenderEngine;

void Surface::InitializeSurface(const VkInstance& _instance, Window* _window, Surface* _output)
{
	_output->instance = _instance;
	_output->window = _window;

	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(_output->window->GetGLFWWindow());
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(_instance, &createInfo, nullptr, &(_output->vkSurface)) != VK_SUCCESS)
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