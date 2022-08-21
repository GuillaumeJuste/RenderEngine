#pragma once

#ifndef RENDERENGINE_WINDOW
#define RENDERENGINE_WINDOW

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

#include "Utils/Event/Event.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class managing GLFW window creation and destruction
	*/
	class Window
	{
	private:
		/**
		 * @brief GLFW window handle
		*/
		GLFWwindow* glfwWindow;

		/**
		 * @brief window width
		*/
		unsigned int width;

		/**
		 * @brief window height
		*/
		unsigned int height;

		/**
		 * @brief window name
		*/
		const char* name;
		
		/**
		 * @brief Callback function called when GLFW window is resized
		 * @param _window window handle
		 * @param _width new window width
		 * @param _height new window height
		*/
		static void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);
	public:

		Utils::Event<> FramebufferResizeEvent;

		Window(unsigned int _width, unsigned int _height, const char* _name);
		~Window() = default;

		/**
		 * @brief window update function
		*/
		void Update();

		/**
		 * @brief cleaning function
		*/
		void Cleanup();

		/**
		 * @brief GLFW window handle getter
		 * @return GLFW window handle
		*/
		GLFWwindow* GetGLFWWindow();

		/**
		 * @brief Get window dimensions
		 * @return window dimensions as VkExtent2D
		*/
		VkExtent2D GetWindowExtent();
	};
}

#endif