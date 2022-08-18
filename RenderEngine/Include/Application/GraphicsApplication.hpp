#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/VulkanContext/VulkanContext.hpp"
#include "Glfw/Window/Window.hpp"
#include "Core/Scene/Scene.hpp"

namespace RenderEngine
{ 
	using namespace Vulkan;
	using namespace Core;

	class GraphicsApplication
	{
	private:
		/**
		 * @brief Window for engine rendering
		*/
		Window* window;

		/**
		 * @brief vulkan context
		*/
		VulkanContext* vulkanContext;

		/**
		 * @brief device used to render scene
		*/
		DeviceContext* deviceContext;

		/**
		 * @brief scene to render
		*/
		Scene scene;

		/**
		 * @brief Initialize Vulkan context
		*/
		void InitVulkan();

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