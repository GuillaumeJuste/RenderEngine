#pragma once

#ifndef RENDERENGINE_SURFACE
#define RENDERENGINE_SURFACE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Window/Glfw/Window.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of a surface matching a glfw window
	*/
	class Surface
	{
	private:
		/**
		 * @brief instance used to initialize the VKSurface
		*/
		VkInstance instance = VK_NULL_HANDLE;

		/**
		 * @brief window from which the surface was created
		*/
		IWindow* window = nullptr;

		/**
		 * @brief Surface
		*/
		VkSurfaceKHR vkSurface = VK_NULL_HANDLE;

	public:

		/// default constructor
		Surface() = default;

		/// default destructor
		~Surface() = default;

		/**
		 * @brief Function initializing the Surface
		 * @param _instance vulkan instance
		 * @param _window glfw window
		 * @param _output surface to initialize
		*/
		static void InitializeSurface(const VkInstance& _instance, IWindow* _window, Surface* _output);

		/**
		 * @brief clean surface
		*/
		void Cleanup();

		/**
		 * @brief get a reference on the VKSurface 
		 * @return reference on the VKSurface 
		*/
		const VkSurfaceKHR& GetVkSurface() const;
	};
}

#endif