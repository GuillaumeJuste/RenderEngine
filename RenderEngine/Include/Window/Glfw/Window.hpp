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

		/**
		 * @brief Get the glew key value for the enum key
		 * @param _key input key enum value
		 * @return glew key value
		*/
		int GetKeyValue(RenderEngine::Utils::Input _key);

		/**
		 * @brief Get the glew status value for the enum status
		 * @param _keyStatus input status enum value
		 * @return glew status value
		*/
		int GetKeyStatusValue(RenderEngine::Utils::InputStatus _keyStatus);
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
		 * @return window dimensions as Mathlib::Vec2
		*/
		Mathlib::Vec2 GetWindowExtent();

		/**
		 * @brief Check the close flag of the window
		 * @return true if the window should be closed
		*/
		bool WindowShouldClose();

		/**
		 * @brief Get the framebuffer size
		 * @param _width output width
		 * @param _height output height
		*/
		void GetFrameBufferSize(int* _width, int* _height);

		/**
		 * @brief Get the window handle as a void*
		 * @return window handle
		*/
		void* GetHandle() const;

		/**
		 * @brief Check the input status of a key if the window is in use
		 * @param _key Key to check
		 * @param _keyStatus state of the key to check
		 * @return true if the key status match _keyStatus
		*/
		bool CheckKeyStatus(RenderEngine::Utils::Input _key, RenderEngine::Utils::InputStatus _keyStatus);

		/**
		 * @brief Get  the cursor position inside the window
		 * @param _mouseX output X coordinate
		 * @param _mouseY outpur Y coordinate
		*/
		void GetCursorPos(double* _mouseX, double* _mouseY);

		/**
		 * @brief Lock the cursor inside the window
		 * @param _enable enabling status
		*/
		void LockMouseToWindow(bool _enable);
	};
}

#endif