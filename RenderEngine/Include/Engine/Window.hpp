#pragma once

#ifndef RENDERENGINE_WINDOW
#define RENDERENGINE_WINDOW

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	class Window
	{
	private:
		GLFWwindow* glfw_window;

		unsigned int width;
		unsigned int height;
		const char* name;
	public:
		Window(unsigned int _width, unsigned int _height, const char* _name);
		~Window() = default;

		void Update();
		void Cleanup();

		GLFWwindow* GetGLFWWindow();
	};
}

#endif