#pragma once

#ifndef RENDERENGINE_WINDOW
#define RENDERENGINE_WINDOW

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"

#include "Utils/Event/Event.hpp"

namespace RenderEngine::Window::GLFW
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

		/**
		 * @brief Event called when window is resized;
		*/
		Utils::Event<> FramebufferResizeEvent;

		/**
		 * @brief Window constructor
		 * @param _width window width
		 * @param _height window height
		 * @param _name window name
		*/
		Window(unsigned int _width, unsigned int _height, const char* _name);

		///default destructor
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