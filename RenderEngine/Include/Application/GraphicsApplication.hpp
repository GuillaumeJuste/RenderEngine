#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/VulkanContext/VulkanContext.hpp"
#include "Engine/Window/Window.hpp"

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