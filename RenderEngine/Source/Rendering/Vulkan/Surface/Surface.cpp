#include "Rendering/Vulkan/Surface/Surface.hpp"

#include <stdexcept>
#include <iostream>


using namespace RenderEngine::Window;
using namespace RenderEngine::Rendering;

void Surface::InitializeSurface(const VkInstance& _instance, IWindow* _window, Surface* _output)
{
	_output->instance = _instance;
	_output->window = _window;

	if (glfwCreateWindowSurface(_instance, reinterpret_cast<GLFWwindow*>(_window->GetHandle()), nullptr, &_output->vkSurface) != VK_SUCCESS)
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