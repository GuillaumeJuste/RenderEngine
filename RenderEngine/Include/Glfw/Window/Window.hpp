#pragma once

#ifndef RENDERENGINE_WINDOW
#define RENDERENGINE_WINDOW

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	class Window
	{
	private:
		GLFWwindow* glfw_window;

		unsigned int width;
		unsigned int height;
		const char* name;
		
		bool frameBufferResized = false;

		static void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);
	public:

		Window(unsigned int _width, unsigned int _height, const char* _name);
		~Window() = default;

		void Update();
		void Cleanup();

		GLFWwindow* GetGLFWWindow();
		const bool& GetFrameBufferResized() const;
		void SetFrameBufferResized(bool _val);

		VkExtent2D GetWindowExtent();
	};
}

#endif