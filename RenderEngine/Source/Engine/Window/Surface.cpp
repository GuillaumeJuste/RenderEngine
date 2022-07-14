#include "Engine/Window/Surface.hpp"

#include <stdexcept>
#include <iostream>

#include "Engine/Window/Window.hpp"

using namespace RenderEngine;

void Surface::InitializeSurface(VkInstance* _instance, Window* _window, Surface* _output)
{
	_output->instance = _instance;
	_output->window = _window;

	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(_output->window->GetGLFWWindow());
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(*_instance, &createInfo, nullptr, &(_output->vkSurface)) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void Surface::Cleanup()
{
	std::cout << "[Cleaning] Surface" << std::endl;

	vkDestroySurfaceKHR(*instance, vkSurface, nullptr);
	
	std::cout << "[Cleaned] Surface" << std::endl;
}

const VkSurfaceKHR& Surface::GetVkSurface() const
{
	return vkSurface;
}