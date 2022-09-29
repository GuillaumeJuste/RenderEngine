#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "Core/Scene/SceneManager.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine
{ 
	using namespace Engine::Vulkan;
	using namespace Core;

	class GraphicsApplication
	{
	private:
		/**
		 * @brief Window for engine rendering
		*/
		GLFW::Window* window;

		/**
		 * @brief vulkan context
		*/
		VulkanContext vulkanContext;

		/**
		 * @brief device used to render scene
		*/
		IDeviceContext* deviceContext;

		/**
		 * @brief render context to render scene
		*/
		IRenderContext* renderContext;
		
		SceneManager SceneManager;

		/**
		 * @brief Initialize engine context
		*/
		void InitEngine();

		/**
		 * @brief Initialize Window
		*/
		void InitWindow();

		/**
		 * @brief Application main loop
		*/
		void MainLoop();

		/**
		 * @brief cleanup fuction to clean variables instances
		*/
		void Cleanup();


		/**
		 * @brief Console print for user selection of the physical device if multiple are eligible
		 * @param _physicalDevicesNames List of the physical devices usable
		 * @return selected physical device
		*/
		std::string UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames);

	public:

		/**
		 * @brief default constructor
		*/
		GraphicsApplication() = default;

		/*
		 * @brief default destructor
		*/
		~GraphicsApplication() = default;

		/**
		 * @brief Run the engine
		*/
		void Run();
	};
}

#endif