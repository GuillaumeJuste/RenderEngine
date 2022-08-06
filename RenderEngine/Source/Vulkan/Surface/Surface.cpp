#include "Vulkan/Surface/Surface.hpp"

#include <stdexcept>
#include <iostream>

using namespace RenderEngine::Vulkan;

void Surface::InitializeSurface(const VkInstance& _instance, Window* _window, Surface* _output)
{
	_output->instance = _instance;
	_output->window = _window;

	if (glfwCreateWindowSurface(_instance, _window->GetGLFWWindow(), nullptr, &_output->vkSurface) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void Surface::Cleanup()
{
	vkDestroySurfaceKHR(instance, vkSurface, nullptr);
	
	std::cout << "[Cleaned] Surface" << std::endl;
}

const VkSurfaceKHR& Surface::GetVkSurface() const
{
	return vkSurface;
}