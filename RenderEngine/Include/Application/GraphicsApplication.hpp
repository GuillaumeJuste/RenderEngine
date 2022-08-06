#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/VulkanContext/VulkanContext.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine
{ 
	using namespace Vulkan;

	class GraphicsApplication
	{
	private:
		Window* window;
		VulkanContext* vulkanContext;

		void InitVulkan();
		void InitWindow();
		void MainLoop();
		void Cleanup();


	public:
		GraphicsApplication() = default;
		~GraphicsApplication() = default;

		void Run();
	};
}

#endif