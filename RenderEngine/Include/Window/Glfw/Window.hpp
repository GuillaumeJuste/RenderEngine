#pragma once

#ifndef RENDERENGINE_WINDOW
#define RENDERENGINE_WINDOW

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Window/IWindow.hpp"

namespace RenderEngine::Window::GLFW
{
	/**
	 * @brief Class managing GLFW window creation and destruction
	*/
	class Window : public RenderEngine::Window::IWindow
	{
	private:
		/**
		 * @brief GLFW window handle
		*/
		GLFWwindow* glfwWindow;

		/**
		 * @brief Callback function called when GLFW window is resized
		 * @param _window window handle
		 * @param _width new window width
		 * @param _height new window height
		*/
		static void FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height);
	public:

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
		 * @brief Get window dimensions
		 * @return window dimensions as VkExtent2D
		*/
		Mathlib::Vec2 GetWindowExtent();

		bool WindowShouldClose();

		void GetFrameBufferSize(int* _width, int* _height);

		void* GetHandle() const;

		bool GetKeyPressed(int _keycode, int _keyStatus);
		void GetCursorPos(double* _mouseX, double* _mouseY);

		void LockMouseToWindow(bool _enable);
	};
}

#endif